#ifndef GESTIONCLIENTS_H
#define GESTIONCLIENTS_H

#include <QObject>

#include <QTcpSocket>
#include <QList>

#include "Client.h"
#include "ServeurTCP.h"
#include "ClientDiscovery.h"
#include "SocketHandler.h"


class GestionClients : public QObject
{
    Q_OBJECT
public:
    explicit GestionClients(QObject *parent = 0);
    ~GestionClients();
    void sendToAll();

    
signals:
    void ClientUploadUpdate(Client *,int);
    void ClientDownloadUpdate(Client *,int);

    void ClientUploadSpeedUpdate(Client *,int);
    void ClientDownloadSpeedUpdate(Client *,int);

    void ClientNumberChanged(int);

    void newFileToUpload(FileStreamer const * fs);
    void newFileToDownload(FileStreamer const * fs);


public slots:
    void DownloadPathUpdate(QString);

private slots:
    void PacketReceived(QByteArray packet, QHostAddress destAddr, QHostAddress senderAddr, bool destJoined);

    void newConnectionRequest(QHostAddress broadcasterAddress,QList<RoutesTableElt> routes);
    void addClients(Client *nextHopClient, QList<RoutesTableElt> routes);
    void newConnectionDone(QTcpSocket *);

    void clientDisconnect();
    void clientConnectionFailed();
    void clientConnected();
    void NewClientConfig(Client *client);

    void broadCastTrigger();
private:
    Client *findClientByPeer(QHostAddress);
    QList<Client*> * findClientsByNextHop(QHostAddress nextHopAdress);

private:
    QList<Client*> _clients;
    QList<SocketHandler*> _socketsHandlers;
    QList<PendingConnectionStruct*> _pendingConnections;

    TCPServer *_listeningServer;
    ClientDiscovery *_clientDiscoveryModule;

    QList<LocalFiles> _localFiles;
    QList<RemoteFiles> _remoteFiles;


    QTimer *_timerBroadcast;
};


#endif // GESTIONCLIENTS_H
