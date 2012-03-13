#include "downloadfoldermodel.h"

#include <QFileDialog>
#include <QVariant>

DownloadFolderModel::DownloadFolderModel( QObject * ui ) :
    ui( ui )
{
}

void DownloadFolderModel::PickDownloadFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( 0, "Choose the downloads folder" );

    if (newFolder != "") {
        emit DownloadFolderChoosed( newFolder );

        ui->setProperty( "downloadFolder", QVariant( newFolder ) );
    }
}
