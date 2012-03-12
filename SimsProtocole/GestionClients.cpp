#include "GestionClients.h"
#include <QMessageBox>

GestionClients::GestionClients(QObject *parent) :
    QObject(parent)
{
    _listeningServer = new TCPServer(this);
    connect(_listeningServer,SIGNAL(AjouterClient(QTcpSocket*)),this,SLOT(newConnectionDone(QTcpSocket*)));
    _clientDiscoveryModule = new ClientDiscovery(this);
    connect(_clientDiscoveryModule,SIGNAL(DatagramReceived(QHostAddress,QList<RoutesTableElt>)),this,SLOT(newConnectionRequest(QHostAddress,QList<RoutesTableElt>)));

    // Gestion de la découverte du réseau
    _timerBroadcast = new QTimer(this);

    _timerBroadcast->setInterval(BROADCAST_INTERVAL);
    _timerBroadcast->setSingleShot(false);
    connect(_timerBroadcast,SIGNAL(timeout()),this,SLOT(broadCastTrigger()));
    _timerBroadcast->start();


    _clients.clear();
}

void GestionClients::newConnectionRequest(QHostAddress broadcasterAddress,QList<RoutesTableElt> routes)
{
    qDebug() << "BroadCasted by" << broadcasterAddress.toString() << "(" << routes.size() << "hosts)";

    Client *broadcasterClient = NULL;

    // première étape : vérifier si l'envoyeur du broadcast est nouveau ou pas.
    bool broadCasterExists = false;
    foreach (Client *client, _clients)
    {
        if (client->peerAddress() == broadcasterAddress)
        {
            broadCasterExists = true;

            broadcasterClient = client;
            if (broadcasterClient->HopNumber()>1)
            {
                _clients.removeOne(broadcasterClient);
                broadcasterClient->deleteLater();
                broadCasterExists = false;
                qDebug() << "Replacing Broadcaster...";
            }
            break;
        }
    }

    //s'il nexiste pas, on le crée (on ne connecte pas le socket tout de suite, afin de pouvoir ajouter les autres entre temps
    if (broadCasterExists == false)
    {
        qDebug() << "Client discovered - Broadcaster";
        //broadcasterClient = new Client(broadcasterAddress);
        QTcpSocket *newClientSocket = new QTcpSocket(this);
        connect(newClientSocket, SIGNAL(connected()), this, SLOT(clientConnected()));
        connect(newClientSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(clientConnectionFailed()));
        PendingConnectionStruct *newPendingConnection = new PendingConnectionStruct;
        newPendingConnection->socket = newClientSocket;
        newPendingConnection->routes = routes;
        _pendingConnections.push_back(newPendingConnection);

        newClientSocket->connectToHost(broadcasterAddress,PORT_SERVEUR);

        return;
    }

    //Récupération des clients connus via cette passerelle
    QList<Client*> * clientsToDelete = findClientsByNextHop(broadcasterAddress);

    //Comparaison clients connus/ clients broadcastés
    foreach (RoutesTableElt route,routes)
    {
        foreach(Client * client, *clientsToDelete)

        if (route.destAddr==client->peerAddress())
        {
            clientsToDelete->removeOne(client);
            break;
        }
    }

    //La liste contient les clients qui n'ont pas été rebroadcastés
    foreach(Client * client, *clientsToDelete)
    {
        _clients.removeOne(client);
        client->deleteLater();
    }

    delete clientsToDelete;

    addClients(broadcasterClient,routes);

}


void GestionClients::addClients(Client *nextHopClient, QList<RoutesTableElt> routes)
{

    // On rajoute toutes les routes en mettant le noeud en nextHop
    foreach(RoutesTableElt newRoute, routes)
    {
        bool routeExists= false;

        //parcourt des clients pour retrouver la route.
        foreach (Client *client, _clients)
        {
            // si la nouvele route est connue
            if (client->peerAddress() == newRoute.destAddr)
            {
                routeExists = true;

                // si la nouvelle route est meilleure, on change celle du client
                if (client->HopNumber() > newRoute.hopNumber)
                {
                    client->UpdateRoute(nextHopClient->socketHandler(), nextHopClient->peerAddress(), newRoute.hopNumber);
                }
                break;
            }
        }

        // si c'est une nouvelle route, on rajoute le client.
        if (routeExists == false)
        {
            qDebug() << "Client discovered - Broadcaster's client";

            Client *client = new Client(nextHopClient->socketHandler(),newRoute.destAddr,nextHopClient->peerAddress(), newRoute.hopNumber);
            NewClientConfig(client);
        }


    }
}





void GestionClients::newConnectionDone(QTcpSocket *socket)
{
    foreach (Client *client, _clients)
    {
        // si la nouvele route est connue
        if (client->peerAddress() == socket->peerAddress())
        {
            // si la nouvelle route est meilleure, on change celle du client
            if (client->HopNumber() > 1)
            {
                qDebug() << "Client updated - TCP connect";;

                // on crée le socketHandler correspondant.
                SocketHandler *newSocketHandler = new SocketHandler(socket);
                connect(newSocketHandler, SIGNAL(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)),this, SLOT(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)));
                _socketsHandlers.push_back(newSocketHandler);


                client->UpdateRoute(newSocketHandler, socket->peerAddress(), 1);
                connect(newSocketHandler,SIGNAL(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)),this,SLOT(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)));
            } else
            {
                // le client éxiste déjà, connexion simultanée...
                socket->disconnectFromHost();
                socket->deleteLater();
            }
            return;
        }
    }

    qDebug() << "Client discovered - TCP connect";

    SocketHandler *newSocketHandler = new SocketHandler(socket);
    connect(newSocketHandler, SIGNAL(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)),this, SLOT(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)));
    _socketsHandlers.push_back(newSocketHandler);

    Client *client = new Client(newSocketHandler);
    NewClientConfig(client);
}


void GestionClients::clientConnected()
{
    QTcpSocket *socket = (QTcpSocket *)sender();
    disconnect(socket,0,this,0);


    // on recherche la liste de routes à ajouter pour la retirer des routes en attente
    foreach (PendingConnectionStruct *pendingConnection, _pendingConnections)
    {
        if (pendingConnection->socket == socket)
        {
            SocketHandler *newSocketHandler = new SocketHandler(socket);
            _socketsHandlers.push_back(newSocketHandler);
            connect(newSocketHandler, SIGNAL(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)), this, SLOT(PacketReceived(QByteArray,QHostAddress,QHostAddress,bool)));

            Client *newClient = new Client(newSocketHandler);
            NewClientConfig(newClient);

            addClients(newClient, pendingConnection->routes);

            _pendingConnections.removeOne(pendingConnection);
            delete pendingConnection;

            break;
        }
    }


}

void GestionClients::NewClientConfig(Client *client)
{
    connect(client,     SIGNAL(Disconnected()),           this, SLOT(clientDisconnect()));
    connect(client,     SIGNAL(newFileToDownload(FileStreamer*)),           this, SIGNAL(newFileToDownload(FileStreamer*)));
    _clients.push_back(client);


    emit ClientNumberChanged(_clients.size());
}


void GestionClients::broadCastTrigger()
{
    _clientDiscoveryModule->sendNewDatagram(_clients);
}


void GestionClients::clientConnectionFailed()
{
    QTcpSocket *socket = (QTcpSocket *)sender();

    // on recherche la liste de routes à ajouter pour la retirer des routes en attente
    foreach (PendingConnectionStruct *pendingConnection, _pendingConnections)
    {
        if (pendingConnection->socket == socket)
        {
            _pendingConnections.removeOne(pendingConnection);
            delete pendingConnection;
            break;
        }
    }

    //on détruit le socket
    socket->deleteLater();

}



void GestionClients::clientDisconnect()
{
    Client *client = (Client *)sender();

    SocketHandler *socketHandler = client->socketHandler();

    // on ne retire le socketHandler que si le client déconnecté est le nextHop
    if ( socketHandler != NULL && client->HopNumber() == 1)
        _socketsHandlers.removeOne(socketHandler);


    _clients.removeOne(client);

    client->deleteLater();


    emit ClientNumberChanged(_clients.size());
}




void GestionClients::sendToAll()
{
    foreach (Client *client, _clients)
    {
        client->SendMessage();
    }
}


void GestionClients::DownloadPathUpdate(QString newPath)
{


}




void GestionClients::PacketReceived(QByteArray packet, QHostAddress destAddr, QHostAddress senderAddr, bool destJoined)
{
    // si nous sommes la destination, pas de traitement spécial : on se contente de transmettre le paquet au
    // client correstpondant à l'adresse du sender
    if (destJoined)
    {
        Client *client = findClientByPeer(senderAddr);
        if (client != NULL)
        {
            client->PacketReceived(packet);
        }
    }
    else
    {
        Client* client = findClientByPeer(destAddr);
        if (client != NULL)
        {
            client->ForwardMessage(packet,destAddr, senderAddr);
        }
    }


}


Client *GestionClients::findClientByPeer(QHostAddress destAddress)
{
    foreach(Client *client, _clients)
    {
        if (client->peerAddress() == destAddress)
            return client;
    }
    return NULL;
}


QList<Client*> * GestionClients::findClientsByNextHop(QHostAddress nextHopAdress)
{
    QList<Client*> * lstClients = new QList<Client*>();
    foreach(Client * client, _clients)
    {
        if (client->nextHopAdress() == nextHopAdress)
            lstClients->push_back(client);
    }

    return lstClients;
}
