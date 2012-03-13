#ifndef UISIGNALINTERFACE_H
#define UISIGNALINTERFACE_H

#include <QObject>

class DownloadFolderModel : public QObject
{
    Q_OBJECT

public:

    DownloadFolderModel( QObject * ui );

    void setFolderPath( QString const & newPath );
    QString folderPath() const;

public slots:

    void PickDownloadFolder();

signals:

    void DownloadFolderChoosed( QString path );

private:

    QString m_folderPath;
    QObject * ui;
};

#endif // UISIGNALINTERFACE_H
