#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "FileIndexing/FileIndexer.h"
#include "SimsProtocole/GestionClients.h"
#include "Wifi/WifiConnection.h"

#include "filelistmodel.h"
#include "availablefileslistmodel.h"
#include "downloadfoldermodel.h"
#include "sharedfolderslistmodel.h"

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


    // INIT WIFI
    WifiConnection wifi;

    // INIT INDEXING
    FileIndexer fileIndexer;

    // INIT PROTOCOL PART
    GestionClients gestionClient(&fileIndexer);

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

    QObject::connect(&sharedFoldersList, SIGNAL(folderAdded(QString)), &fileIndexer, SLOT(addDirectory(QString)));
    QObject::connect(&sharedFoldersList, SIGNAL(folderRemoved(QString)), &fileIndexer, SLOT(removeDirectory(QString)));

    // Gestion connexion/deconnexion
    QObject::connect(&wifi, SIGNAL(Connected()), &gestionClient, SLOT(StartBroadcast()));
    QObject::connect(&wifi, SIGNAL(Disconnected()), &gestionClient, SLOT(StopBroadcast()));
//    QObject::connect(&wifi, SIGNAL(Connected()), &bouton, SLOT(Connected())); //Etat du bouton : 'Connexion'
//    QObject::connect(&wifi, SIGNAL(Disconnected()), &bouton, SLOT(Disconnected())); //Etat du bouton : 'Deconnexion'
//    QObject::connect(&wifi, SIGNAL(ConectionFail()), &bouton, SLOT(ConectionFail())); //Etat du bouton : 'Connexion'
//    QObject::connect(&bouton, SIGNAL(ConnectClicked()), &wifi, SLOT(Connect()));
//    QObject::connect(&bouton, SIGNAL(DisconnectClicked()), &wifi, SLOT(Disconnect()));
//    QObject::connect(&bouton, SIGNAL(ConnectClicked()), &bouton, SLOT(Connecting())); //Etat du bouton : 'Connexion' grisé
//    QObject::connect(&bouton, SIGNAL(DisconnectClicked()), &bouton, SLOT(Disconnecting())); //Etat du bouton : 'Deconnexion' grisé


    // Connexion au reseau WiFi AdHoc
    wifi.Connect();

    return app->exec();
}
