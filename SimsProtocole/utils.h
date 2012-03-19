#ifndef UTILS_H
#define UTILS_H


#include <QString>
#include <QHostAddress>
#include <QTcpSocket>

class Client;

#define PORT_SERVEUR 50855
#define DISCOVERY_PORT 50854

#define BROADCAST_INTERVAL 10000
#define BLOCK_SIZE 50000


struct LocalFiles
{
    QString path;
    QString filename;
    quint32 size;
};

struct RemoteFiles
{
    QString filename;
    quint32 size;
    Client *client;
};


struct RoutesTableElt
{
    QHostAddress destAddr;
    quint8 hopNumber;
};

struct PendingConnectionStruct
{
    QTcpSocket *socket;
    QList<RoutesTableElt> routes;
};


enum TYPE_PAQUET { FILE_REQUEST, FILE_REQUEST_INIT, FILE_REQUEST_ACK, FILE_DATA, LIST_REQUEST, LIST_DATA };


enum CLIENT_STATE { IDLE, SENDING_FILE, WAITING_ACK};

#endif // UTILS_H
