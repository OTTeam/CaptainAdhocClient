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

    void AddFolder( QString const & folderPath );

    void RemoveFolder( int index );

    QStringList FoldersList() const;

public slots:

    void AddFolder();

signals:

    void folderAdded( QString folder );
    void folderRemoved( QString folder );

private:

    static QString const propertyName;

    QStringList folderList;
    QDeclarativeContext * qmlRootContext;
};

#endif // SHAREDFOLDERSLISTMODEL_H
