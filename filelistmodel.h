#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include <QAbstractListModel>

#include "FileReceivedModel.h"

class FileStreamer;

class FileListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum FileRole{
        Name = Qt::UserRole + 1,
        Hash,
        Type,
        Size,
        Progress,
        Speed
    };

    FileListModel( QObject * parent = 0 );

    int rowCount( QModelIndex const & parent = QModelIndex() ) const;

    QVariant data( QModelIndex const & index, int role = Qt::DisplayRole ) const;

    Q_INVOKABLE int count(){ return rowCount(); }

public slots:

    void AddFile( FileStreamer const * fileStreamer );
    void RemoveFile( HashType hash );

    void DownloadFile( HashType hash );

private slots:

    void fileChanged();

signals:

    void DownloadRequested( /* arguments to be specified */ );

private:

    int hashToIndex( HashType hash );

private:

    QList< FileReceivedModel * > m_files;
};

#endif // DOWNLOADLISTMODEL_H
