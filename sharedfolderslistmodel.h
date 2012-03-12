#ifndef SHAREDFOLDERSLISTMODEL_H
#define SHAREDFOLDERSLISTMODEL_H

#include <QObject>

#include <QStringList>
#include <QDeclarativeContext>

class SharedFoldersListModel : public QObject
{
    Q_OBJECT

public:

    SharedFoldersListModel( QDeclarativeContext * qmlRootContext );

    QStringList FoldersList() const;

public slots:

    void AddFolder();
    void AddFolder( QString const & folderPath );
    void RemoveFolder( int index );

signals:

    void folderAdded( QString folder );
    void folderRemoved( QString folder );

private:

    static QString const propertyName;

    QStringList folderList;
    QDeclarativeContext * qmlRootContext;
};

#endif // SHAREDFOLDERSLISTMODEL_H
