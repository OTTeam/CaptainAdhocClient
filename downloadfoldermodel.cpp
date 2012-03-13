#include "downloadfoldermodel.h"

#include <QFileDialog>
#include <QVariant>

DownloadFolderModel::DownloadFolderModel( QObject * ui ) :
    ui( ui )
{
}

void DownloadFolderModel::setFolderPath( QString const & newFolder )
{
    if (newFolder != "")
    {
        emit DownloadFolderChoosed( newFolder );

        ui->setProperty( "downloadFolder", QVariant( newFolder ) );
    }
}

QString DownloadFolderModel::folderPath() const
{
    return m_folderPath;
}

void DownloadFolderModel::PickDownloadFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( 0, "Choose the downloads folder" );

    setFolderPath( newFolder );
}
