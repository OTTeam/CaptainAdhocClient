#ifndef FILESTREAMER_H
#define FILESTREAMER_H

#include <QObject>
#include <QHostAddress>
#include <QFile>
#include <QTimer>


enum FILESTREAMER_TYPE { UPLOAD_STREAMER, DOWNLOAD_STREAMER };


class FileStreamer : public QObject
{
    Q_OBJECT
public:
    explicit FileStreamer(QObject *parent = 0);
    FileStreamer(QString filePath, QString destAddr, QString senderAddr, quint64 fileSize, FILESTREAMER_TYPE type );
    ~FileStreamer();

    QByteArray nextPacket();
    void writeNext(QByteArray Packet);

    quint64 fileSize() const;
    QString fileName() const;

    QByteArray id();
    
signals:
    void EndOfFile();
    void progressUpdate( quint64 currentSize, float speed );

private slots:
    void timerProgressTimeout();

private:
    QByteArray _id;
    QFile*  _fileToStream;
    QString _fileName;
    quint64 _fileSize;

    quint64 _bytesWritten;
    quint64 _PreviousBytesWritten;


    QString _destAddr;
    QString _senderAddr;

    FILESTREAMER_TYPE _type;

    QTimer *_progressTimer;
};

#endif // FILESTREAMER_H
