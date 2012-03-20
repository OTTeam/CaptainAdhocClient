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
        _fileSize = _fileToStream->size();
    }
    else
    {
        _fileSize = fileSize;
        _fileToStream->open(QIODevice::WriteOnly | QIODevice::Truncate);
    }

    _bytesWritten = 0;

    _destAddr = destAddr;
    _senderAddr = senderAddr;

    _fileName = fileInfo.fileName();
    // on génère un hash du nom du fichier afin de pouvoir le reconnaitre simplement par la suite.
    // ici, on ne cherche pas la sécurité, juste un moyen d'indexation, on peut donc utiliser du Md5 peu sécure
    _id = _fileName.toAscii();//QCryptographicHash::hash(_fileName.toUtf8(), QCryptographicHash::Md5);


    _bytesWritten = 0;
    _PreviousBytesWritten = 0;

    _progressTimer = new QTimer(this);
    _progressTimer->setInterval(500);
    connect(_progressTimer,SIGNAL(timeout()),this,SLOT(timerProgressTimeout()));
    _progressTimer->start();
}


FileStreamer::~FileStreamer()
{
    if (_fileToStream!= NULL && _fileToStream->isOpen())
    {
        _fileToStream->close();
        if (_type==DOWNLOAD_STREAMER && _bytesWritten< _fileSize)
        {
            _fileToStream->remove();
        }
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
            qint64 headerPos = out.device()->pos();
            out << (quint16) 0;
            out << typePacket;
            out << _id;
            out.writeRawData(data.data(),data.size());

            out.device()->seek(0);
            out << (quint16) (paquet.size() - sizeof(quint16));

            out.device()->seek(headerPos);
            out << (quint16) (paquet.size() - headerPos - sizeof(quint16));


            qDebug() << "SENDING to" << _destAddr << "- total size :" << (quint16) (paquet.size() - sizeof(quint16))
                     << "data size" << paquet.size() - headerPos - sizeof(quint16);

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

    //quint64 percCompletion = (100* _bytesWritten)/_fileSize;

    quint64 bytesDiff = _bytesWritten - _PreviousBytesWritten;

    float newSpeed = (bytesDiff*1000.0)/_progressTimer->interval();

    emit progressUpdate(_bytesWritten, newSpeed);

    _PreviousBytesWritten = _bytesWritten;
}


QString FileStreamer::id()
{
    return _id;
}




quint64 FileStreamer::fileSize() const
{
    return _fileSize;
}

QString FileStreamer::fileName() const
{
    return _fileName;
}
