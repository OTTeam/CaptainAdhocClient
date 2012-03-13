#ifndef SOCKETHANDLER_H
#define SOCKETHANDLER_H

#include <QObject>
#include <QTcpSocket>
#include <QHostAddress>

#include "FileStreamer.h"


class SocketHandler : public QObject
{
    Q_OBJECT
public:
    explicit SocketHandler(QObject *parent = 0);
    SocketHandler(QTcpSocket *socket);
    ~SocketHandler();
    void SendPacket(QByteArray packet);
    void SendFile(FileStreamer *file);
    void stopSending(FileStreamer *file);

    QHostAddress peerAddress();
    QHostAddress localAddress();
signals:
    void PacketReceived(QByteArray packet, QHostAddress senderAddr, QHostAddress destAddr, bool DestJoined);
    void Disconnected();

private slots:
    void SocketReceivedData();
    void SocketBytesWritten(qint64);

    void fileSendingComplete();

private:
    QTcpSocket* _socket;
    QList<QByteArray> _packetsInLine;
    QList<FileStreamer*> _filesInLine;
    quint16 _currentPacketSize;

    FileStreamer * _lastFS;
    
};

#endif // SOCKETHANDLER_H
