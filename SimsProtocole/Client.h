#ifndef CLIENT_H
#define CLIENT_H


#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QList>
#include <QFile>
#include <QTimer>

#include "FileStreamer.h"
#include "SocketHandler.h"
#include "utils.h"

// Gestion de la liste de fichiers partagée par ce client
#include "../FileReceivedModel.h"



class Client : public QObject
{
    Q_OBJECT
public:
    Client(SocketHandler *Socket);
    Client(SocketHandler *Socket, QHostAddress dest, QHostAddress nextHop, quint8 HopNumber);


    Client(QHostAddress Address);      // a retirer, constructeur de debug

    ~Client();
    QHostAddress peerAddress();
    QHostAddress nextHopAdress();
    quint8 HopNumber();
    SocketHandler *socketHandler();

    void UpdateRoute(SocketHandler *s,QHostAddress nextHop, quint8 newHopNumber);
    void SendMessage();

    void ForwardMessage(QByteArray data, QHostAddress destAdd, QHostAddress senderAdd);

    QList< FileReceivedModel * > FileReceivedList();
private:
    void configClient();

    void receivedFileRequest(QByteArray packet);     // Demande de fichier de la part du client
    void receivedFileRequestInit(QByteArray packet); // Réponse du serveur lors de la demande (confirmation nom, taille)
    void receivedFileRequestAck(QByteArray packet);  // Confirmation du client
    void receivedFileData(QByteArray packet);        // Réception d'un fichier par le client
    void receivedListRequest(QByteArray packet);     // Demande de Liste de fichiers
    void receivedListData(QByteArray packet);        // Liste

signals:
    void NewData(int);

    void Disconnected();

     void newFileToDownload(const FileStreamer*);
     void newFileToUpload(const FileStreamer*);

     void FileListUpdated(Client *);
     void FileListDeleted(Client *);

public slots:
    void PacketReceived(QByteArray packet);


private slots:
    void fileUploadingComplete();
    void fileDownloadingComplete();

private:
    SocketHandler* _socketHandler;
    quint8 _hopNumber;

    QHostAddress _peerAddr;
    QHostAddress _nextHop;

    QList<FileStreamer*> _filesUploading;
    QList<FileStreamer*> _filesDownloading;

    CLIENT_STATE _etat;

    QList< FileReceivedModel *> _availableFiles;
};

#endif // CLIENT_H
