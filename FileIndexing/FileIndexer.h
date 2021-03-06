#ifndef FILEINDEXER_H
#define FILEINDEXER_H

#include <QObject>
#include <QDir>
#include <QQueue>
#include "FileIndexDao.h"
#include "FolderDao.h"
#include <QFuture>
#include <QFutureWatcher>
#include <QMap>

class FileIndexer : public QObject
{
    Q_OBJECT

public:
    FileIndexer(bool computeHash = false);
    FileIndexer(QSqlDatabase db, bool computeHash = false);

    ~FileIndexer();

    qint32 indexDirectory(const QDir& dir);

    void deleteDirectory(const QDir& dir);

    bool createDatabase();

    QList<FileModel> getAllIndexedFiles();

    QList<SimpleFileModel*> getSharedFiles();

    SimpleFileModel* getFile(const QString& hash);

    QList<FileModel> searchFiles(const QString& keyword);

    void setDatabase(QSqlDatabase db) { _dao.setDatabase(db); _folderDao.setDatabase(db); }

    void setNameFilters(const QStringList& nameFilters) { _nameFilters = nameFilters; }

    bool isComputeHash() const { return _computeHash; }
    void setComputeHash(bool computeHash) { _computeHash = computeHash; }

    QMap<QString, SimpleFileModel*> * fileMap();

public slots:
    void addDirectory(const QString& path);
    void removeDirectory(const QString& path);
    void indexingFinished();
    void removingFinished();

private:
    FileIndexDao _dao;
    FolderDao _folderDao;
    QQueue<QDir> _pendingDirs;
    QQueue<QDir> _pendingDeleteDirs;
    QDir _currentFolder;
    QStringList _nameFilters;
    QList<FileModel> _indexedFiles;

    bool _computeHash;

    bool _stopIndexation;

    QFuture<void> _indexingResult;
    QFutureWatcher<void> _indexingWatcher;

    QFuture<void> _removingResult;
    QFutureWatcher<void> _removingWatcher;

    bool _indexing;

    QMap<QString, SimpleFileModel*> _fileMap;

    bool indexFile(const QFileInfo& fileInfo, const QDir& dir);

    void startIndexingDirectory(const QDir& dir);
    void startRemovingDirectory(const QDir& dir);

    void indexNextDirectory();
    void removeNextDirectory();

    void cancelIndexation();
};

#endif // FILEINDEXER_H
