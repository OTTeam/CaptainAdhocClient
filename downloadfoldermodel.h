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

    // TODO rajouter un champ pour mettre un truc par defaut

    QObject * ui;
};

#endif // UISIGNALINTERFACE_H
