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




class Client : public QObject
{
    Q_OBJECT
public:
    Client(SocketHandler *Socket);
    Client(SocketHandler *Socket, QHostAddress dest, QHostAddress nextHop, quint8 HopNumber);


    Client(QHostAddress Address);      // a retirer, constructeur de debug

    ~Client();
    QHostAddress peerAddress();
    quint8 HopNumber();
    SocketHandler *socketHandler();

    void UpdateRoute(SocketHandler *s,QHostAddress nextHop, quint8 newHopNumber);
    void SendMessage();

    void ForwardMessage(QByteArray data, QHostAddress destAdd, QHostAddress senderAdd);
private:
    void configClient();

    void receivedFileRequest(QDataStream &in);     // demande de fichier de la part du client
    void receivedFileRequestInit(QDataStream &in); // réponse du serveur lors de la demande (confirmation nom, taille)
    void receivedFileRequestAck(QDataStream &in);  // confirmation du client
    void receivedFileData(QDataStream &in);        // réception d'un fichier par le client
    void receivedFileList(QDataStream &in);

signals:
    void NewData(int);

    void Disconnected();

     void newFileToDownload(FileStreamer*);

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

    quint64 _bytesReceived;
    quint64 _previousBytesReceived;

    quint64 _bytesSent;
    quint64 _previousBytesSent;

    CLIENT_STATE _etat;

};

#endif // CLIENT_H
