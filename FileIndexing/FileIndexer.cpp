#include <QDebug>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QSqlError>
#include <QtConcurrentRun>

#include "FileIndexer.h"
#include "FileUtils.h"

FileIndexer::FileIndexer(bool computeHash) : _computeHash(computeHash), _indexing(false), _stopIndexation(false)
{
    qDebug() << "createDatabase result : " << createDatabase();

    _dao = FileIndexDao(QSqlDatabase::database("fileDb"));
    _folderDao = FolderDao(QSqlDatabase::database("fileDb"));

    connect(&_indexingWatcher, SIGNAL(finished()), this, SLOT(indexingFinished()));
    connect(&_removingWatcher, SIGNAL(finished()), this, SLOT(removingFinished()));
}

FileIndexer::FileIndexer(QSqlDatabase db, bool computeHash) : _dao(db), _folderDao(db), _computeHash(computeHash), _indexing(false), _stopIndexation(false)
{
    connect(&_indexingWatcher, SIGNAL(finished()), this, SLOT(indexingFinished()));
}

bool FileIndexer::createDatabase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "fileDb");
    db.setDatabaseName("files.sqlite");
    if (!db.open()) {
        qCritical(qPrintable(db.lastError().text()));
        return false;
    }

    if (!FileIndexDao::createTable(db, true)) {
        qCritical(qPrintable(db.lastError().text()));
        return false;
    }

    if (!FolderDao::createTable(db, true)) {
        qCritical(qPrintable(db.lastError().text()));
        return false;
    }

    return true;
}

void FileIndexer::addDirectory(const QString& path)
{
    qDebug() << "Adding directory " << path << " to queue";
    _pendingDirs.enqueue(QDir(path));
    if (!_indexing) {
        //Start indexing
        indexNextDirectory();
    }
}

void FileIndexer::removeDirectory(const QString &path)
{
    QDir dir(path);
    //CAS 1. Dossier en attente d'indexation (easy)
    if (_pendingDirs.contains(dir)) {
        qDebug() << "Directory pending for indexation : removing from queue";
        _pendingDirs.removeAll(dir);
        return;
    }

    //CAS 3. Dossier en cours d'indexation (hard)
    if (_indexing && dir == _currentFolder) {
        qDebug() << "Current directory " << path << " is being removed";
        _pendingDeleteDirs.enqueue(dir);
        cancelIndexation();
        return;
    }

    //CAS 2. Dossier déjà indexé (medium)
    //Attendre fin indexation en cours (voir indexingFinished())
    if (!_indexing) {
        //Si pas en train d'indexer un dossier, on peut effectuer la suppression
        qDebug() << "Deleting directory " << path << " from index";
        deleteDirectory(dir);
        return;
    }
    //Sinon il faut attendre la fin de l'indexation
    _pendingDeleteDirs.enqueue(dir);
}

void FileIndexer::indexingFinished()
{
    qDebug() << "Finished indexing dir " << _currentFolder.absolutePath();

    _indexing = false;
    if (!_pendingDeleteDirs.isEmpty()) {
        qDebug() << "removing next directory...";
        removeNextDirectory();
    } else if (!_pendingDirs.isEmpty()) {
        qDebug() << "indexing next directory...";
        indexNextDirectory();
    }
}

void FileIndexer::removingFinished()
{
    qDebug() << "Finished removing dir " << _currentFolder.absolutePath();
    _indexing = false;
    if (!_pendingDeleteDirs.isEmpty()) {
        qDebug() << "removing next directory...";
        removeNextDirectory();
    } else if (!_pendingDirs.isEmpty()) {
        qDebug() << "indexing next directory...";
        indexNextDirectory();
    }
}

QList<FileModel> FileIndexer::searchFiles(QString keyword)
{
    QString kw = keyword;
    bool wildcard = (kw.indexOf('%') > -1 || kw.indexOf('*') > -1);
    kw.replace('_', "/_").replace('*', '%').replace('?', '_');
    qDebug() << "keyword = " << kw;
    return _dao.searchFiles(kw, wildcard);
}

QMap<QString, SimpleFileModel> FileIndexer::fileMap()
{
    if (_fileMap.isEmpty()) {
        getSharedFiles();
    }
    return _fileMap;
}

/*!
  * Index all files in dir and its subdirectories
  */
qint32 FileIndexer::indexDirectory(const QDir& dir)
{
    qint32 count = 0;
    FolderModel folder;
    folder.setPath(dir.absolutePath());
    _folderDao.insertFolder(folder);

    QDirIterator it(dir.absolutePath(), _nameFilters, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext() && !_stopIndexation) {
        QString path = it.next();
        QFileInfo info = it.fileInfo();
        //Add the file to the index
        if (indexFile(info, dir))
            count++;
    }
    _stopIndexation = false;
    return count;
}

void FileIndexer::deleteDirectory(const QDir &dir)
{
    qDebug() << "deleteDirectory " << dir.absolutePath();
    qint32 count = _dao.deleteWholeFolder(dir.absolutePath());
    qDebug() << "Removed " << count << " files";
}

QList<FileModel> FileIndexer::getAllIndexedFiles()
{
    return _dao.getAllFiles();
}

QList<SimpleFileModel> FileIndexer::getSharedFiles()
{
    QList<FileModel> list = getAllIndexedFiles();
    QList<SimpleFileModel> files;
    foreach (FileModel model, list) {
        SimpleFileModel simpleModel = model.toSimpleFileModel();
        files << simpleModel;
        _fileMap.insert(simpleModel.hash(), simpleModel);
    }
    return files;
}

bool FileIndexer::indexFile(const QFileInfo& fileInfo, const QDir& dir)
{
    qDebug() << "Indexing file " << fileInfo.absoluteFilePath();
    FileModel model(fileInfo.fileName(), dir.absolutePath(), fileInfo.absoluteDir().absolutePath(), fileInfo.absoluteFilePath(), fileInfo.size(), _computeHash ? FileUtils::fileMd5Hash(fileInfo.absoluteFilePath()) : "");
    return _dao.insertFile(model);
}

void FileIndexer::startIndexingDirectory(const QDir &dir)
{
    _currentFolder = dir;
    _indexing = true;
    qDebug() << "Indexing directory " << _currentFolder.absolutePath();
    _indexingResult = QtConcurrent::run(this, &FileIndexer::indexDirectory, _currentFolder);
    _indexingWatcher.setFuture(_indexingResult);
}

void FileIndexer::startRemovingDirectory(const QDir &dir)
{
    _currentFolder = dir;
    qDebug() << "Removing directory " << _currentFolder.absolutePath();
    _removingResult = QtConcurrent::run(this, &FileIndexer::deleteDirectory, _currentFolder);
    _removingWatcher.setFuture(_removingResult);
}

void FileIndexer::indexNextDirectory()
{
    QDir dir = _pendingDirs.dequeue();
    startIndexingDirectory(dir);
}

void FileIndexer::removeNextDirectory()
{
    QDir dir = _pendingDeleteDirs.dequeue();
    startRemovingDirectory(dir);
}

void FileIndexer::cancelIndexation()
{
    _stopIndexation = true;
}
