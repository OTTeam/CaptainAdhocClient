#include "filelistmodel.h"

#include "SimsProtocole/FileStreamer.h"
#include "FileReceivedModel.h"

FileListModel::FileListModel( QObject * parent )
    : QAbstractListModel( parent )
{
    QHash< int, QByteArray > roles;

    roles[ Name ] = "name";
    roles[ Type ] = "type";
    roles[ Size ] = "size";
    roles[ Progress ] = "progress";
    roles[ Speed ] = "speed";

    setRoleNames( roles );
}

void FileListModel::AddFile( FileStreamer const * fileStreamer )
{
    FileReceivedModel * newFile = new FileReceivedModel( fileStreamer );

    connect( newFile, SIGNAL( progressChanged() ),
             this, SLOT( fileChanged() ) );
    connect( newFile, SIGNAL( speedChanged() ),
             this, SLOT( fileChanged() ) );

    beginInsertRows( QModelIndex(), rowCount(), rowCount() );
    m_files << newFile;
    endInsertRows();
}

void FileListModel::RemoveFile( HashType hash )
{
    int fileIdx = hashToIndex( hash );

    beginRemoveRows( QModelIndex(), fileIdx, fileIdx );
    FileReceivedModel * fileToDelete = m_files.takeAt( fileIdx );
    endRemoveRows();

    delete fileToDelete;
}

void FileListModel::DownloadFile( HashType hash )
{
    emit DownloadRequested();

    RemoveFile( hash );
}

int FileListModel::rowCount( const QModelIndex & parent ) const
{
    return m_files.count();
}

QVariant FileListModel::data(const QModelIndex & index, int role) const
{
    if ( index.row() < 0 || index.row() > m_files.count() )
        return QVariant();

    FileReceivedModel const * file = m_files[ index.row() ];

    switch( role )
    {
    case Name:
        return file->name();
        break;

    case Type:
        return file->type();
        break;

    case Size:
        return file->size();
        break;

    case Progress:
        return file->progress();
        break;

    case Speed:
        return file->speed();
        break;
    }

    return QVariant();
}

void FileListModel::fileChanged()
{
    // retrieve signal sender
    // send dataChanged for this file only

    // for now : redraw everything (dirty)
    reset();
}

int FileListModel::hashToIndex( HashType hash )
{
    // look the list of files to find the right hash
    for( int i = 0 ; i < m_files.count() ; i++ )
    {
        if( m_files[ i ]->hash() == hash )
        {
            return i;
        }
    }

    return -1;
}
