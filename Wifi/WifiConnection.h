#ifndef WIFICONNECTION_H
#define WIFICONNECTION_H

#include <QObject>
#include "WifiManager.h"

// SSID du reseau :
#define ADHOC_SSID "CAPTAIN_ADHOC"

// Mot de passe du reseau :
#define ADHOC_PWD "archibald1941"

class WifiConnection : public QObject
{
    Q_OBJECT

public:
    WifiConnection();
    virtual ~WifiConnection();

public slots:
    void Connect();
    void Disconnect();

signals:
    void Connected();
    void Disconnected();
    void ConnectionFail();

private slots:
    void onConnectionStatusChanged(int);

private:
    WifiManager * _manager;
    WifiNetwork * _network;

    bool _connected;
};

#endif // WIFICONNECTION_H
