#include "FileStreamer.h"

#include <QFileInfo>
#include <QCryptographicHash>
#include "utils.h"
#include <QByteArray>
#include <QString>

FileStreamer::FileStreamer(QObject *parent) :
    QObject(parent)
{

}


FileStreamer::FileStreamer(QString filePath, QString destAddr, QString senderAddr, quint64 fileSize, FILESTREAMER_TYPE type)
{
    _fileToStream = new QFile(filePath);
    QFileInfo fileInfo(filePath);
    _type = type;
    if (_type == UPLOAD_STREAMER)
    {
        _fileToStream->open(QIODevice::ReadOnly);
        _fileSize = fileInfo.size();
    }
    else
    {
        _fileSize = fileSize;
        _fileToStream->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }

    _bytesWritten = 0;

    _destAddr = destAddr;
    _senderAddr = senderAddr;

    if (_fileToStream->atEnd())
        emit EndOfFile();

    _fileName = fileInfo.fileName();
    // on g�n�re un hash du nom du fichier afin de pouvoir le reconnaitre simplement par la suite.
    // ici, on ne cherche pas la s�curit�, juste un moyen d'indexation, on peut donc utiliser du Md5 peu s�cure
    _id = QCryptographicHash::hash(_fileName.toUtf8(), QCryptographicHash::Md5);


    _bytesWritten = 0;
    _PreviousBytesWritten = 0;

    _progressTimer = new QTimer(this);
    _progressTimer->setInterval(500);
    _progressTimer->start();

}


FileStreamer::~FileStreamer()
{
    if (_fileToStream!= NULL && _fileToStream->isOpen())
    {
        _fileToStream->close();
        delete _fileToStream;
        _fileToStream = NULL;
    }
}


QByteArray FileStreamer::nextPacket()
{

    QByteArray paquet;

    if (_type == UPLOAD_STREAMER)
    {


        quint16 typePacket = FILE_DATA;


        QDataStream out(&paquet, QIODevice::WriteOnly);

        QByteArray data = _fileToStream->read(BLOCK_SIZE);
        _bytesWritten += data.size();

        // si on a pas atteint la fin du fichier
        if (data.length() > 0)
        {
            out << (quint16) 0;
            out << _destAddr;
            out << _senderAddr;
            out << (quint16) (sizeof(typePacket) + _id.size() + data.size());
            out << typePacket;
            out << _id;
            out << data;

            out.device()->seek(0);
            out << (quint16) (paquet.size() - sizeof(quint16));

            qDebug() << "SENDING to" << _destAddr << "- packet size :" << (quint16) (paquet.size() - sizeof(quint16));

        } else
        {
            qDebug() << "SENDING complete";
            _fileToStream->close();
            delete _fileToStream;
            _fileToStream=NULL;
            _bytesWritten = 0;
            emit EndOfFile();
        }
    }

    return paquet;

}


void FileStreamer::writeNext(QByteArray Packet)
{

    if (_fileToStream->isWritable())
    {
       _fileToStream->write(Packet);
       _bytesWritten += Packet.size();

       if (_bytesWritten == _fileSize)
       {
           qDebug() << "RECEIVING complete";
           _fileToStream->close();
           delete _fileToStream;
           _fileToStream=NULL;

           _bytesWritten = 0;

           emit EndOfFile();
       }
    }

}

void FileStreamer::timerProgressTimeout()
{
    quint64 bytesDiff = (100* _bytesWritten)/_fileSize;

    float newSpeed = bytesDiff*_progressTimer->interval()*0.001;

    emit progressUpdate(bytesDiff, newSpeed);

    _PreviousBytesWritten = _bytesWritten;
}


QByteArray FileStreamer::id()
{
    return _id;
}




quint64 FileStreamer::fileSize()
{
    return _fileSize;
}