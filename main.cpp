#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "SimsProtocole/GestionClients.h"

#include "filelistmodel.h"
#include "availablefileslistmodel.h"
#include "downloadfoldermodel.h"
#include "sharedfolderslistmodel.h"

#include "FileIndexing/FileIndexer.h"

#include <QObject>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QList>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    // VARIABLES
    FileListModel downloads;
    //AvailableFilesListModel availableFiles;


    // INIT PROTOCOL PART
    GestionClients gestionClient(0);


    // INIT INDEXING
    FileIndexer fileIndexer;

    // INIT QML PART
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/UI/CaptainAdhocUI.qml"));
    viewer.showExpanded();

    QDeclarativeContext * rootContext   = viewer.rootContext();
    QObject * qmlRootObject = viewer.rootObject();

    DownloadFolderModel downloadFolder( qmlRootObject );
    gestionClient.DownloadPathUpdate( downloadFolder.folderPath() );

    SharedFoldersListModel sharedFoldersList( rootContext );
    sharedFoldersList.AddFolder( "C:\\" ); // as default. Should be read from saved settings.

    rootContext->setContextProperty( "downloadsList", &downloads );
    //rootContext->setContextProperty( "availableFilesList", &availableFiles );

    // CONNECT ALL THE SIGNALS
    QObject::connect( &gestionClient, SIGNAL( newFileToDownload( FileStreamer const * ) ),
                      &downloads, SLOT( AddFile( const FileStreamer*) ) );

    QObject::connect( &downloadFolder, SIGNAL( DownloadFolderChoosed( QString ) ),
                      &gestionClient, SLOT( DownloadPathUpdate( QString ) ) );


    QObject::connect( qmlRootObject, SIGNAL( pickDownloadFolder() ),
                      &downloadFolder, SLOT( PickDownloadFolder() ) );

    QObject::connect( qmlRootObject, SIGNAL( pickSharedDir() ),
                      &sharedFoldersList, SLOT( AddFolder() ) );

    QObject::connect( qmlRootObject, SIGNAL( delSharedDir( int ) ),
                      &sharedFoldersList, SLOT( RemoveFolder( int ) ) );


    QObject::connect(&sharedFoldersList, SIGNAL(folderAdded(QString)),
                     &fileIndexer, SLOT(addDirectory(QString)));

    QObject::connect(&sharedFoldersList, SIGNAL(folderRemoved(QString)),
                     &fileIndexer, SLOT(removeDirectory(QString)));

    return app->exec();
}
