#include <QHostInfo>

#include "ClientDiscovery.h"
#include "utils.h"

ClientDiscovery::ClientDiscovery(QObject *parent) :
    QObject(parent)
{

    _socket = new QUdpSocket(this);
    _socket->bind(DISCOVERY_PORT,QUdpSocket::ShareAddress);
    connect(_socket,SIGNAL(readyRead()),this,SLOT(newDatagramAvailable()));
    qsrand(5000);
}


void ClientDiscovery::newDatagramAvailable()
{
    //on vient de recevoir un nouveau datagram, on vérifie
    //que c'est bien un datagram de notre programme
    // puis on émet le signal d'ajout de client si nécessaire
    while (_socket->hasPendingDatagrams())
    {
        // récupération du datagram reçu
        QByteArray datagram;
        QHostAddress senderAddress;
        datagram.resize(_socket->pendingDatagramSize());
        _socket->readDatagram(datagram.data(), datagram.size(),&senderAddress);

        //QString text = datagram.data();

        // création d'un datastream pour lire plus simplement
        QDataStream in(datagram);

//        qDebug()<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" ;
//        qDebug()<< "BROADCAST RECEIVE" ;


//        qDebug() << ">>>>> senderAddress" << senderAddress.toString() << ">>>>>>";

        qint16 RouteListSize = 0;

        quint64 packetAlea;
        in >> packetAlea;


        // on est le sender
        if (packetAlea == _lastPacketAlea)
            return;



        in >> RouteListSize;
//        qDebug() << "RouteList size : " << RouteListSize;
        // on place les adresses reçues dans une liste
        QList<RoutesTableElt> routesReceived;
        routesReceived.clear();

        QHostInfo hostInfo = QHostInfo::fromName(QHostInfo::localHostName());

        for(int i = 0; i<RouteListSize; i++)
        {
            RoutesTableElt newElt;

            QString destAddrStr;
            QString nextHopAddrStr;
            in >> destAddrStr;
            in >> nextHopAddrStr;
            in >> newElt.hopNumber;

            newElt.destAddr = destAddrStr;
            newElt.nextHopAddr = nextHopAddrStr;

            // on incrémente le hop number car on a la passerelle en plus
            newElt.hopNumber++;

            //Si on nous donne une route pour se contacter nous même, on l'ignore
            if (newElt.destAddr == _socket->localAddress() || hostInfo.addresses().contains(newElt.destAddr))
            {
                continue;
            }

            //Si on nous donne une route dont le next hop est nous même
            // on l'ignore aussi, pour ne pas créer de boucle de routage
            if (newElt.nextHopAddr == _socket->localAddress() || hostInfo.addresses().contains(newElt.nextHopAddr))
            {
                qDebug () << "Ignoring route for " << newElt.destAddr.toString() << ", next hop was Self";
                continue;
            }

            routesReceived.push_back(newElt);
//          qDebug() << "AddressStr :" << destAddrStr << "Address :" << newElt.destAddr << " -- Hop :" << newElt.hopNumber;

        }


        bool localSent = false;
//        qDebug() << "************* Local ***************";
        if (hostInfo.addresses().contains(senderAddress))
        {
            localSent = true;
        }
//        foreach (QHostAddress add, hostInfo.addresses())
//        {
////            qDebug() << add.toString();
//            if (senderAddress == add)
//            {
//                localSent = true;
//            }
//        }
        if (senderAddress == _socket->localAddress())
            localSent = true;
//        qDebug() << _socket->localAddress().toString();
//        qDebug() << "***********************************";
//        qDebug()<< ">>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>" ;

        if (localSent == false  && _socket->localAddress() != QHostAddress::Null)
            emit DatagramReceived(senderAddress,routesReceived);

    }

}


void ClientDiscovery::sendNewDatagram(QList<Client *> routesList )
{

    QByteArray datagram;
    QDataStream out(&datagram, QIODevice::WriteOnly);

    // écriture du nombre de routes

    // écriture de la liste des routes

//    qDebug()<< "BS---------------------------------------" ;
//    qDebug()<< "BROADCAST SEND" ;
    qDebug() << "BROADCASTING...";

    _lastPacketAlea = qrand();

    out << _lastPacketAlea;
    out << (quint16) routesList.size();
//    qDebug()<< "routesList Count : " << routesList.count();
    foreach(Client *client, routesList)
    {
        out << client->peerAddress().toString();
        out << client->nextHopAdress().toString();
        out << client->HopNumber();
//        qDebug() << "Address :" << client->address().toString() << " -- hop :" << client->hopNumber();
    }
//    qDebug()<< "---------------------------------------BS" ;
    _socket->writeDatagram(datagram.data(), datagram.size(),
                          QHostAddress::Broadcast, DISCOVERY_PORT);

}
