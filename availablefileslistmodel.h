#ifndef AVAILABLEFILESLISTMODEL_H
#define AVAILABLEFILESLISTMODEL_H

#include <QAbstractListModel>
#include <QHash>
#include <QList>
#include <QString>

class FileReceivedModel;

class AvailableFilesListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    AvailableFilesListModel();

public slots:

    //AddFile

private:
    QList< QString > hashList;
};

#endif // AVAILABLEFILESLISTMODEL_H
