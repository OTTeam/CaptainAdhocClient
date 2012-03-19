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
                                       QVariant::fromValue( completeList ) );
}

void AvailableFilesListsHandler::FileListUpdated( Client * client )
{
    int clientIdx = clients.indexOf( client );

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

    completeList.clear();

    for( int i = 0 ; i < clients.count() ; i++ )
    {
        QList< FileReceivedModel * > fileList = clients[ i ]->FileReceivedList();

        for( int j = 0 ; j < fileList.count() ; j++ )
        {
            completeList.append( static_cast< QObject * >( fileList.at( j ) ) );
        }
    }

    uiRootContext->setContextProperty( propertyName, QVariant::fromValue( completeList ) );
}
