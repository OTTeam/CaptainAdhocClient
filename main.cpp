#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "SimsProtocole/GestionClients.h"

#include "FileReceivedModel.h"
#include "downloadlistmodel.h"

#include <QObject>
#include <QDeclarativeContext>
#include <QGraphicsObject>
#include <QList>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    // VARIABLES
    QList< QObject * > downloads;


    // INIT PROTOCOL PART
    GestionClients gestionClient(0);


    // INIT QML PART
    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/UI/CaptainAdhocUI.qml"));
    viewer.showExpanded();

    QDeclarativeContext * rootContext   = viewer.rootContext();

    rootContext->setContextProperty( "downloadsModel", QVariant::fromValue( downloads ) );

    //rootContext->setProperty( "downloadingFilesList", QVariant::fromValue( downloadingFilesList) );

    // CONNECT ALL THE SIGNALS
//    QObject::connect( &gestionClient,
//                      SIGNAL( ClientDownloadSpeedUpdate( Client*,int ) ),
//                      )

    return app->exec();
}
