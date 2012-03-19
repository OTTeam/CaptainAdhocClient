#ifndef AVAILABLEFILESLISTSHANDLER_H
#define AVAILABLEFILESLISTSHANDLER_H

#include <QObject>
#include <QDeclarativeContext>
#include <QList>

#include "SimsProtocole/utils.h"
#include "SimsProtocole/GestionClients.h"

#include "FileReceivedModel.h"

class AvailableFilesListsHandler : public QObject
{
    Q_OBJECT

public:

    AvailableFilesListsHandler( GestionClients * clientsManager,
                                QDeclarativeContext * uiRootContext );

private:

    void UpdateView();

public slots:

    void FileListUpdated( Client * client );
    void FileListDeleted( Client * client );

    void DownloadRequestedFromView( Client * client, HashType fileHash );

signals:

    void DownloadRequested( Client * fromClient, HashType fileHash );

private:

    GestionClients * clientsManager;

    QDeclarativeContext * uiRootContext;
    static const QString propertyName;

    QList< Client * > clients;
};

#endif // AVAILABLEFILESLISTSHANDLER_H
