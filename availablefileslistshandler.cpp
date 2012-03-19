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
    UpdateView();
}

void AvailableFilesListsHandler::FileListDeleted( Client * client )
{
    UpdateView();
}

void AvailableFilesListsHandler::UpdateView()
{
    // concatene les listes des differents clients et les envoie à la vue pour
    // affichage

    //QList< Client * > clients = clientsManager
}
