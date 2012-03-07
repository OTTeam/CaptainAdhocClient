#ifndef CLIENT_H
#define CLIENT_H


#include <QTcpSocket>
#include <QHostAddress>
#include <QString>
#include <QList>
#include <QFile>
#include <QTimer>
#include "utils.h"




class Client : public QObject
{
    Q_OBJECT
public:
    Client(QTcpSocket *Socket);
    Client(QHostAddress Address);
    Client(QTcpSocket *Socket, QHostAddress dest, QHostAddress nextHop, quint8 HopNumber);
    ~Client();
    QString ReceivingFileName();
    quint64 ReceivingFileSize();
    QHostAddress Address();
    quint8 HopNumber();
    QTcpSocket *Socket();
    void UpdateRoute(QTcpSocket *s,QHostAddress nextHop, quint8 newHopNumber);
    void ConnectSocket();
    void SendMessage();

    void ForwardMessage(QHostAddress senderAdd,QHostAddress destAdd, QByteArray data);
private:
    void configClient();

    void receivedFileRequest();     // demande de fichier de la part du client
    void receivedFileRequestInit(); // réponse du serveur lors de la demande (confirmation nom, taille)
    void receivedFileRequestAck();  // confirmation du client
    void receivedFileData();        // réception d'un fichier par le client
    void receivedFileList();

signals:
    void NewData(int);
    void UploadProgressUpdate(int);
    void DownloadProgressUpdate(int);

    void DownloadSpeedUpdate(int);
    void UploadSpeedUpdate(int);

    void Connected();
    void SocketError();
    void Disconnected();

public slots:
    void newBytesReceived();
    void newBytesWritten(qint64);

private slots:
    void dlSpeedMeasure();
    void ulSpeedMeasure();

private:
    QTcpSocket *_socket;
    quint8 _hopNumber;

    QHostAddress _dest;
    QHostAddress _nextHop;

    quint16 _messageLength;

    quint64 _bytesReceived;
    quint64 _previousBytesReceived;

    quint64 _filesize;

    QFile *_fileToReceive;
    QFile *_fileToSend;

    quint64 _bytesSent;
    quint64 _previousBytesSent;

    CLIENT_STATE _etat;

    QTimer *_timerDlSpeed;
    QTimer *_timerUlSpeed;

};

#endif // CLIENT_H
