#ifndef FILEINDEXER_H
#define FILEINDEXER_H

#include <QObject>
#include <QDir>
#include <QList>
#include "FileIndexDao.h"
#include "FolderDao.h"


class FileIndexer : public QObject
{
    Q_OBJECT

public:
    FileIndexer(bool computeHash = false);
    FileIndexer(QSqlDatabase db, bool computeHash = false);

    void addDirectory(QDir dir);

    void removeDirectory(QDir dir);

    bool createDatabase();

    qint32 updateDatabase();

    QList<FileModel> getAllIndexedFiles();

    QList<SimpleFileModel> getSharedFiles();

    QList<FileModel> searchFiles(QString keyword);

    void setDatabase(QSqlDatabase db) { _dao.setDatabase(db); _folderDao.setDatabase(db); }

    void setNameFilters(const QStringList& nameFilters) { _nameFilters = nameFilters; }

    bool isComputeHash() const { return _computeHash; }
    void setComputeHash(bool computeHash) { _computeHash = computeHash; }

public slots:
    void addDirectory(const QString& path);
    void removeDirectory(const QString& path);

private:
    FileIndexDao _dao;
    FolderDao _folderDao;
    QList<QDir> _directories;
    QStringList _nameFilters;
    bool _computeHash;

    qint32 indexDirectory(const QDir& dir);

    bool indexFile(const QFileInfo& fileInfo, const QDir& dir);
};

#endif // FILEINDEXER_H
