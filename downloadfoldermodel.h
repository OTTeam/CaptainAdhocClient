#ifndef UISIGNALINTERFACE_H
#define UISIGNALINTERFACE_H

#include <QObject>

class DownloadFolderModel : public QObject
{
    Q_OBJECT

public:

    DownloadFolderModel( QObject * ui );

public slots:

    void PickDownloadFolder();

signals:

    void DownloadFolderChoosed( QString path );

private:

    QObject * ui;
};

#endif // UISIGNALINTERFACE_H
