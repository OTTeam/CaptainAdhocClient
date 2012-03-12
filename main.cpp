#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "SimsProtocole/GestionClients.h"

#include "FileReceivedModel.h"
#include "downloadlistmodel.h"
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
    DownloadListModel downloads;


    // INIT PROTOCOL PART
    GestionClients gestionClient(0);


    // INIT QML PART
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/UI/CaptainAdhocUI.qml"));
    viewer.showExpanded();

    QDeclarativeContext * rootContext   = viewer.rootContext();
    QObject * qmlRootObject = viewer.rootObject();

    DownloadFolderModel downloadFolder( qmlRootObject );
    SharedFoldersListModel sharedFoldersList( rootContext );
    sharedFoldersList.AddFolder( "C:\\" );

    rootContext->setContextProperty( "downloadsList", &downloads );

    // CONNECT ALL THE SIGNALS
    QObject::connect( qmlRootObject, SIGNAL( pickDownloadFolder() ),
                      &downloadFolder, SLOT( PickDownloadFolder() ) );
    QObject::connect( qmlRootObject, SIGNAL( pickSharedDir() ),
                      &sharedFoldersList, SLOT( AddFolder() ) );

    return app->exec();
}
