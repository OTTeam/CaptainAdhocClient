#include "availablefileslistshandler.h"

AvailableFilesListsHandler::AvailableFilesListsHandler
(
    GestionClients * clientsManager,
    QDeclarativeContext * uiRootContext
) :
    clientsManager( clientsManager ),
    uiRootContext( uiRootContext )
{
}

void AvailableFilesListsHandler::FileListUpdated( Client * client )
{
    int clientIdx = clients.indexOf( client );

    if( clientIdx == -1 )
    {
        clients << client;
    }
    else
    {
        clients.replace( clientIdx, client );
    }

    UpdateView();
}

void AvailableFilesListsHandler::FileListDeleted( Client * client )
{
    clients.removeOne( client );

    UpdateView();
}

void AvailableFilesListsHandler::UpdateView()
{
    // concatene les listes des differents clients et les envoie à la vue pour
    // affichage

    //QList< Client * > clients = clientsManager
}
