#include "SocketHandler.h"

SocketHandler::SocketHandler(QObject *parent) :
    QObject(parent)
{
}


SocketHandler::SocketHandler(QTcpSocket *socket, QObject *parent):
    QObject(parent)
{
    _socket = socket;
    _currentPacketSize = 0;

    _packetsInLine.clear();


    connect(_socket, SIGNAL(readyRead()), this, SLOT(SocketReceivedData()));
    connect(_socket, SIGNAL(bytesWritten(qint64)), this, SLOT(SocketBytesWritten(quint64)));

    connect(_socket, SIGNAL(disconnected()), this, SIGNAL(Disconnected()));
}


SocketHandler::~SocketHandler()
{
    while(!_filesInLine.isEmpty())
    {
        FileStreamer* fs = _filesInLine.first();
        delete fs;
    }

}


void SocketHandler::SendPacket(QByteArray packet)
{
    if (_packetsInLine.size() == 0 && _socket->bytesToWrite() == 0)
    {
        _socket->write(packet);
    }
    else
    {
        _packetsInLine.push_back(packet);
    }
}

void SocketHandler::SendFile(FileStreamer *file)
{
    _filesInLine.push_back(file);
    // on force le prochain envoi
    if (_socket->bytesToWrite() == 0)
    {
        SocketBytesWritten(0);
    }
}


QHostAddress SocketHandler::peerAddress()
{
    return _socket->peerAddress();
}


QHostAddress SocketHandler::localAddress()
{
    return _socket->localAddress();
}


void SocketHandler::SocketReceivedData()
{
    QDataStream in(_socket);

    QHostAddress destAddr;
    QHostAddress senderAddr;

    while (_socket->bytesAvailable() > 0)
    {
        if (_currentPacketSize == 0)
        {
            if (_socket->bytesAvailable() < sizeof(quint16))
                return;

            in >> _currentPacketSize;
        }
        // si le paquet n'est pas entier, on passe
        if (_socket->bytesAvailable() < _currentPacketSize)
            return;

        // sinon, on lit les adresses de source et Destination pour savoir ce qu'il faut en faire
        QString destAddStr;
        QString senderAddStr;

        in >> destAddStr;
        in >> senderAddStr;

        destAddr = destAddStr;
        senderAddr = senderAddStr;

        qDebug() << "RECEIVED packet size" << _currentPacketSize << "from" << senderAddStr << "to" << destAddStr;

        //        qDebug() << destAddStr;

        //        qDebug() << senderAddStr;

        //        qDebug() << senderSocket->localAddress();

        // Lecture du paquet en entier :
        quint16 dataSize;
        in >> dataSize;

        QByteArray packet;
        packet.resize(dataSize);
        in.readRawData(packet.data(),dataSize);


        _currentPacketSize = 0;

        emit PacketReceived(packet, senderAddr, destAddr, (_socket->localAddress() == destAddr) );

    }

}



void SocketHandler::SocketBytesWritten(quint64 bytesWritten)
{
    Q_UNUSED(bytesWritten)

    if (_socket->bytesToWrite() == 0)
    {
        if ( !_packetsInLine.isEmpty() )
        {
            _socket->write(_packetsInLine.first());
            _packetsInLine.removeFirst();
            return;
        }

        if ( !_filesInLine.isEmpty() )
        {
            FileStreamer *fileStreamer = (FileStreamer*) _filesInLine.first();
            QByteArray packet = fileStreamer->nextPacket();
            if (! packet.isEmpty())
                _socket->write(packet);
        }

    }
}

void SocketHandler::fileSendingComplete()
{
    FileStreamer* filestreamer = (FileStreamer*) sender();

    _filesInLine.removeOne(filestreamer);
}
