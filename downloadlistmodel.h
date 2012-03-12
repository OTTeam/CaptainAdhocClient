#ifndef DOWNLOADLISTMODEL_H
#define DOWNLOADLISTMODEL_H

#include <QAbstractListModel>

class Client;
class FileReceivedModel;

class DownloadListModel : public QAbstractListModel
{
    Q_OBJECT

public:

    enum DownloadRoles{
        Name = Qt::UserRole + 1,
        Type,
        Size,
        Progress,
        Speed
    };

    DownloadListModel( QObject * parent = 0 );

    int rowCount( QModelIndex const & parent = QModelIndex() ) const;

    QVariant data( QModelIndex const & index, int role = Qt::DisplayRole ) const;

    Q_INVOKABLE int count(){ return rowCount(); }

public slots:

    void AddDownload( Client const * client );

private slots:

    void fileChanged();

private:

    QList< FileReceivedModel * > m_files;
};

#endif // DOWNLOADLISTMODEL_H
