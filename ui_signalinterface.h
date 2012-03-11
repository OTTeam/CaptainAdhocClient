#ifndef UISIGNALINTERFACE_H
#define UISIGNALINTERFACE_H

#include <QObject>

class UISignalInterface : public QObject
{
    Q_OBJECT

public:

    UISignalInterface( QObject * ui );

public slots:

    void PickDownloadFolder();

signals:

    void DownloadFolderChoosed( QString path );

private:

    QObject * ui;
};

#endif // UISIGNALINTERFACE_H
