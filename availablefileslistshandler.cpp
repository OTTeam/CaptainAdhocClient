#include "availablefileslistshandler.h"

QString const AvailableFilesListsHandler::propertyName = "availableFilesList";

AvailableFilesListsHandler::AvailableFilesListsHandler
(
    GestionClients * clientsManager,
    QDeclarativeContext * uiRootContext
) :
    clientsManager( clientsManager ),
    uiRootContext( uiRootContext )
{
    // init view with an empty list.
    uiRootContext->setContextProperty( propertyName,
                                       QVariant::fromValue( QList< FileReceivedModel * >() ) );
}

void AvailableFilesListsHandler::FileListUpdated( Client * client )
{
    int clientIdx = clients.indexOf( client );

    QList< FileReceivedModel * > filesFromClient = client->FileReceivedList();

    if( clientIdx == -1 )
    {
        clients << client;
    }

    UpdateView();
}

void AvailableFilesListsHandler::FileListDeleted( Client * client )
{
    clients.removeOne( client );

    UpdateView();
}

void AvailableFilesListsHandler::DownloadRequestedFromView( Client * client, HashType fileHash )
{
    // trouver le client a partir du hash et lui demander de telecharger le
    // fichier.

    client->RequestFile( fileHash );
}

void AvailableFilesListsHandler::UpdateView()
{
    // concatene les listes des differents clients et les envoie à la vue pour
    // affichage

    QList< FileReceivedModel * > completeList;

    for( int i = 0 ; i < clients.count() ; i++ )
    {
        completeList += clients[ i ]->FileReceivedList();
    }

    uiRootContext->setContextProperty( propertyName, completeList );
}
