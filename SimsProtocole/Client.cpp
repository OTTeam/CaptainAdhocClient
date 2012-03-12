#include <QHostAddress>

#include "utils.h"
#include "Client.h"
#include <QMessageBox>
#include <QFile>
#include <QFileDialog>
#include <QTime>


/*
 * Constructeur appelé lorsque les serveur TCP a reçu
 * une nouvelle demande de connexion client
 */
Client::Client(SocketHandler *s)
{
    _socketHandler = s;
    // dans ce cas là, comme la connexion a été demandée par le socket
    // on sait que c'est à 1 hop
    _peerAddr = s->peerAddress();
    _nextHop = s->peerAddress();
    _hopNumber = 1;

    qDebug() << "New client - Dest :" << _peerAddr.toString() << "- nextHop :" << _nextHop.toString();
//    qDebug() << "Client(QTCpSocket *)";
//    qDebug() << "nextHop" << _nextHop << "  dest" << _dest;


    configClient();
}

/*
 * Constructeur appelé lors de l'ajout d'un client
 */
Client::Client(SocketHandler *s, QHostAddress dest, QHostAddress nextHop, quint8 hopNumber)
{
    _socketHandler = s;
    _peerAddr = dest;
    _nextHop = nextHop;
    _hopNumber = hopNumber;

   qDebug() << "New client - Dest :" << _peerAddr.toString() << "- nextHop :" << _nextHop.toString();

    configClient();
}


/*
 * Constructeur appelé lors de l'ajout d'un client par son adresse.
 */
Client::Client(QHostAddress address)
{
    _socketHandler = new SocketHandler(this);
    configClient();
    _nextHop = address;
    _peerAddr = address;


    qDebug() << "New client - Dest :" << _peerAddr.toString() << "- nextHop :" << _nextHop.toString();


    _hopNumber = 1;
    //_socket->connectToHost(address,PORT_SERVEUR);
}


void Client::configClient()
{
    connect(_socketHandler, SIGNAL(Disconnected()),      this, SIGNAL(Disconnected()));

    _etat = IDLE;
}


Client::~Client()
{

    while (!_filesUploading.isEmpty())
    {
        FileStreamer* fs = _filesUploading.takeFirst();
        delete fs;
    }
    while (!_filesDownloading.isEmpty())
    {
        FileStreamer* fs = _filesDownloading.takeFirst();
        delete fs;
    }

    qDebug() << "Deleted client - Dest :" << _peerAddr.toString() << "- nextHop :" << _nextHop.toString();

    // on détruit le socket seulement si on est le next hop (pas si c'est une passerelle)
    if (_peerAddr == _nextHop)
        delete _socketHandler;
}

/*
 * Slot appelé à chaque fois que la socket reçoit des données.
 * Première étape : vérifier le nombre d'octets devant être lus.
 * Si on le connait déjà et que tout est arrivé, on lit le message
 *
 * Un message commence par un type (FILE_REQUEST_INIT, FILE_REQUEST, LIST_REQUEST)
 * FILE_REQUEST_INIT : 2 données : nom du fichier à recevoir, taille
 * FILE_REQUEST      : le fichier à recevoir (découpé), data brut.
 * LIST_REQUEST      : anuaire de fichiers disponibles : sous la forme nomFic1, taille1, nomFic2, Taille2....
 *
*/
void Client::PacketReceived(QByteArray packet)
{
    // Si tout va bien, on continue : on récupère le message
    QDataStream in(&packet, QIODevice::ReadOnly);


    // Si ces lignes s'exécutent, c'est qu'on a reçu tout le message : on peut le récupérer !
    quint16 type;
    in >> type;

    switch (type)
    {
    case FILE_REQUEST_INIT:
        qDebug() << "PACKET is FILE_REQUEST_INIT";
        receivedFileRequestInit(in);
        break;
    case FILE_DATA:
        qDebug() << "PACKET is FILE_DATA";
        receivedFileData(in);
        break;
    case FILE_REQUEST_ACK:
        qDebug() << "PACKET is FILE_REQUEST_ACK";
        receivedFileRequestAck(in);
        break;
    case LIST_REQUEST:
        qDebug() << "PACKET is LIST_REQUEST";
        receivedFileList(in);
        break;
    default:
        break;
    }
}

void Client::fileUploadingComplete()
{
    FileStreamer* filestreamer = (FileStreamer*) sender();

    _filesUploading.removeOne(filestreamer);

    filestreamer->deleteLater();
}


void Client::fileDownloadingComplete()
{
    FileStreamer* filestreamer = (FileStreamer*) sender();

    _filesDownloading.removeOne(filestreamer);

    filestreamer->deleteLater();
}

void Client::receivedFileRequest(QDataStream &in)
{

}


void Client::receivedFileRequestInit(QDataStream &in)
{
    // ici on a juste envoyé le filename, on le récupère donc
    QString fileRequested;
    quint16 fileSize;
    in >> fileRequested;
    in >> fileSize;

    QString path = QFileDialog::getExistingDirectory(0,"Enregistrer le fichier sous...");
    path += "\\" + fileRequested;

    // création du fileStreamer
    FileStreamer *fileStreamer = new FileStreamer(path,_peerAddr.toString(), _socketHandler->localAddress().toString(), fileSize, DOWNLOAD_STREAMER);
    connect(fileStreamer, SIGNAL(EndOfFile()), this, SLOT(fileDownloadingComplete()));
    _filesDownloading.push_back(fileStreamer);
    emit newFileToDownload(fileStreamer);

    //    _fileToReceive = new QFile(path);
//    _fileToReceive->open(QIODevice::WriteOnly | QIODevice::Truncate);

//    _bytesReceived = 0;


    // on envoie alors le Ack pour confirmer au serveur l'envoi du fichier
    QByteArray paquet;

    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    quint16 type = FILE_REQUEST_ACK;
    out << (quint16) 0;                                 // taillePaquet globale que l'on changera après écriture du paquet
    out << _peerAddr.toString();                        //la destination du paquet
    out << _socketHandler->localAddress().toString();   // l'expéditeur du paquet (nous même)
    out << (quint16) (sizeof(type)+ fileStreamer->id().size()); // taille du data, ici c'est juste type, du coup pas de traitement
    out << type;                                        // typePaquet
    out << fileStreamer->id();                           //id du fichier

    // mise à jour de taillePaquet
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));



    qDebug() << "SENDING ACK to" << _peerAddr.toString() << "from" <<  _socketHandler->localAddress().toString() << "- packet size :" << (quint16) (paquet.size() - sizeof(quint16));


    _socketHandler->SendPacket(paquet); // On envoie le paquet

}


void Client::receivedFileRequestAck(QDataStream &in)
{
    _etat = SENDING_FILE;
    QByteArray id;
    in >> id;
    FileStreamer* fileStreamerAck = NULL;
    //on retrouve le bon fileHandler
    foreach(FileStreamer* filestreamer, _filesUploading)
    {
        if (filestreamer->id() == id)
                fileStreamerAck = filestreamer;
    }

    if (fileStreamerAck != NULL)
        _socketHandler->SendFile(fileStreamerAck);
}


void Client::receivedFileData(QDataStream &in)
{
    QByteArray fileId;
    QByteArray data;

    in >> fileId;
    in >> data;

    bool fileFound = false;
    foreach (FileStreamer* fileStreamer, _filesDownloading)
    {
        if (fileStreamer->id() == fileId)
        {
            fileStreamer->writeNext(data);
            fileFound = true;
            break;
        }
    }

}

void Client::receivedFileList(QDataStream &in)
{


}


/*
 * Fonction d'envoi de message, on lui passe en paramètre le fichier à envoyer et elle se charge de
 * le découper et de le passer par la socket. (envoi au préalable d'un message : FILE_REQUEST_INIT pour
 * annoncer la venue du fichier
*/
void Client::SendMessage()
{
    QByteArray paquet;
    QDataStream out(&paquet, QIODevice::WriteOnly);

    // On prépare le paquet à envoyer
    quint16 type = FILE_REQUEST_INIT;

    // le fichier à envoyer, uniquement à des fins de test
    QString filePath = QFileDialog::getOpenFileName(0,"Sélectionnez le fichier à envoyer");

    // création du filestreamer pour l'envoi du fichier (celui ci sera supprimé si le ack n'arrive pas)
    FileStreamer* fileStreamer = new FileStreamer(filePath, _peerAddr.toString(), _socketHandler->localAddress().toString(), 0, UPLOAD_STREAMER);
    connect(fileStreamer, SIGNAL(EndOfFile()), this, SLOT(fileDownloadingComplete()));
    _filesUploading.push_back(fileStreamer);


    // ouverture du fichier
    //    _fileToSend = new QFile(filePath);
    //    _fileToSend->open(QIODevice::ReadOnly);

    // changement d'état pour pouvoir envoyer la suite lors de l'appel au slot "donneesEcrites"
    _etat = WAITING_ACK;
    _bytesSent=0;

    QString SendFilename = fileStreamer->fileName();
    quint64 SendFilesize = fileStreamer->fileSize();
    quint64 posData;
    quint16 headerSize;

    out << (quint16) 0;
    out << _peerAddr.toString();
    out << _socketHandler->localAddress().toString();

    posData = paquet.size();
    out << (quint16) 0;    // taillePaquet que l'on changera après écriture du paquet
    headerSize = paquet.size();

    out << type;           // typePaquet
    out << SendFilename;   // NomFichier
    out << SendFilesize;   //TailleFichier

    qDebug() << "SENDING InitFileRequest to" << _peerAddr.toString() << " - packet size" << (quint16) (paquet.size() - sizeof(quint16));


    // mise à jour de taillePaquet globale
    out.device()->seek(0);
    out << (quint16) (paquet.size() - sizeof(quint16));


    // puis de la taille du paquet data (sans le header
    out.device()->seek(posData);
    out << (quint16) (paquet.size() - headerSize);


    QMessageBox::information(0,"sending", SendFilename);
    _socketHandler->SendPacket(paquet); // On envoie le paquet
}



void Client::ForwardMessage(QByteArray data, QHostAddress destAdd, QHostAddress senderAdd)
{
    // On commence par lire le paquet, sans l'interpréter.
    // On renvoie alors le paquet complet à notre next-hop
    quint16 dataSize = data.size();

    QByteArray packetToSend;
    QDataStream out(&packetToSend,QIODevice::WriteOnly );

    out << (quint16) 0;
    out << destAdd.toString();
    out << senderAdd.toString();
    out << dataSize;
    out << data;

    out.device()->seek(0);
    out << (quint16) (packetToSend.size() - sizeof(quint16));

    qDebug() << "FORWARDING to" << destAdd.toString() << " - packet size :" << (quint16) (packetToSend.size() - sizeof(quint16));

    _socketHandler->SendPacket(packetToSend); // On envoie le paquet
}

QHostAddress Client::nextHopAdress()
{
    return _nextHop;
}

QHostAddress Client::peerAddress()
{
    return _peerAddr;
}

quint8 Client::HopNumber()
{
    return _hopNumber;
}

SocketHandler *Client::socketHandler()
{
    return _socketHandler;
}

void Client::UpdateRoute(SocketHandler *s,QHostAddress nextHop, quint8 newHopNumber)
{
    // on déconnecte le socket de ce client
    disconnect(_socketHandler,0,this,0);
    _socketHandler = s;
    connect(_socketHandler,SIGNAL(Disconnected()),this,SIGNAL(Disconnected()));
    _nextHop = nextHop;
    _hopNumber = newHopNumber;

}
