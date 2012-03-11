#include "ui_signalinterface.h"

#include <QFileDialog>
#include <QVariant>

UISignalInterface::UISignalInterface( QObject * ui ) :
    ui( ui )
{
}

void UISignalInterface::PickDownloadFolder()
{
    QString newFolder = QFileDialog::getExistingDirectory( 0, "Choose the downloads folder" );

    emit DownloadFolderChoosed( newFolder );

    ui->setProperty( "downloadFolder", QVariant( newFolder ) );
}
