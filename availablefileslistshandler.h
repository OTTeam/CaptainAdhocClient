#ifndef AVAILABLEFILESLISTSHANDLER_H
#define AVAILABLEFILESLISTSHANDLER_H

#include <QObject>
#include <QDeclarativeContext>

#include "SimsProtocole/utils.h"
#include "SimsProtocole/GestionClients.h"

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

signals:

    void DownloadRequested( Client * fromClient, HashType fileHash );

private:

    GestionClients * clientsManager;
    QDeclarativeContext * uiRootContext;
};

#endif // AVAILABLEFILESLISTSHANDLER_H
