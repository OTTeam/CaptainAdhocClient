#include <QDebug>
#include <QDirIterator>
#include <QSqlDatabase>
#include <QSqlError>

#include "FileIndexer.h"
#include "FileUtils.h"

FileIndexer::FileIndexer(bool computeHash) : _computeHash(computeHash)
{
    qDebug() << "createDatabase result : " << createDatabase();

    _dao = FileIndexDao(QSqlDatabase::database());
    _folderDao = FolderDao(QSqlDatabase::database());
}

FileIndexer::FileIndexer(QSqlDatabase db, bool computeHash) : _dao(db), _folderDao(db), _computeHash(computeHash)
{
}

void FileIndexer::addDirectory(QDir dir)
{
    if (!_directories.contains(dir)) {
        _directories << dir;
        qDebug() << "Directory " << dir.absolutePath() << " added";
    }
}

void FileIndexer::removeDirectory(QDir dir)
{
    _directories.removeAll(dir);
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
    addDirectory(QDir(path));
}

void FileIndexer::removeDirectory(const QString &path)
{
    qDebug() << "Directory " << path << " removed";
    removeDirectory(QDir(path));
}

qint32 FileIndexer::updateDatabase()
{
    qint32 count = 0;
    qDebug() << "Indexing " << _directories.size() << " directories";
    foreach (QDir dir, _directories) {
        count += indexDirectory(dir);
    }
    return count;
}

QList<FileModel> FileIndexer::searchFiles(QString keyword)
{
    QString kw = keyword;
    bool wildcard = (kw.indexOf('%') > -1 || kw.indexOf('*') > -1);
    kw.replace('_', "/_").replace('*', '%').replace('?', '_');
    qDebug() << "keyword = " << kw;
    return _dao.searchFiles(kw, wildcard);
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
    while (it.hasNext()) {
        QString path = it.next();
        QFileInfo info = it.fileInfo();
        //Add the file to the index
        if (indexFile(info, dir))
            count++;
    }
    return count;
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
        files << model.toSimpleFileModel();
    }
    return files;
}

bool FileIndexer::indexFile(const QFileInfo& fileInfo, const QDir& dir)
{
    qDebug() << "Indexing file " << fileInfo.absoluteFilePath();
    FileModel model(fileInfo.fileName(), dir.absolutePath(), fileInfo.absoluteDir().absolutePath(), fileInfo.absoluteFilePath(), fileInfo.size(), _computeHash ? FileUtils::fileMd5Hash(fileInfo.absoluteFilePath()) : "");
    return _dao.insertFile(model);
}
