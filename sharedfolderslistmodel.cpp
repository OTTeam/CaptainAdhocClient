#include "sharedfolderslistmodel.h"

#include <QFileDialog>

QString const SharedFoldersListModel::propertyName( "sharedFoldersListModel" );

SharedFoldersListModel::SharedFoldersListModel( QDeclarativeContext * qmlRootContext ) :
    qmlRootContext( qmlRootContext )
{
    qmlRootContext->setContextProperty( propertyName, QVariant::fromValue( folderList ) );
}

void SharedFoldersListModel::AddFolder()
{
    AddFolder( QFileDialog::getExistingDirectory( 0, "Choose a folder to share" ) );
}

void SharedFoldersListModel::AddFolder( QString const & folderPath )
{
    if( !folderList.contains( folderPath, Qt::CaseInsensitive ) )
    {
        folderList.append( folderPath );
        qmlRootContext->setContextProperty( propertyName, QVariant::fromValue( folderList ) );
        emit folderAdded( folderPath );
    }
}

void SharedFoldersListModel::RemoveFolder( int index )
{
    if( index >= 0 && index < folderList.count() )
    {
        QString removedPath = folderList[ index ];
        folderList.removeAt( index );
        qmlRootContext->setContextProperty( propertyName, QVariant::fromValue( folderList ) );
        emit folderRemoved( removedPath );
    }
}

QStringList SharedFoldersListModel::FoldersList() const
{
    return folderList;
}
