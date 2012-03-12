#include "downloadlistmodel.h"

#include "SimsProtocole/FileStreamer.h"
#include "FileReceivedModel.h"

DownloadListModel::DownloadListModel( QObject * parent )
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

void DownloadListModel::AddDownload( FileStreamer const * fileStreamer )
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

int DownloadListModel::rowCount( const QModelIndex & parent ) const
{
    return m_files.count();
}

QVariant DownloadListModel::data(const QModelIndex & index, int role) const
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

void DownloadListModel::fileChanged()
{
    // retrieve signal sender
    // send dataChanged for this file only

    // for now : redraw everything (dirty)
    reset();
}
