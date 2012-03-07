#include <QtGui/QApplication>
#include "qmlapplicationviewer.h"

#include "SimsProtocole/GestionClients.h"

#include <QGraphicsObject>

Q_DECL_EXPORT int main(int argc, char *argv[])
{
    QScopedPointer<QApplication> app(createApplication(argc, argv));

    QmlApplicationViewer viewer;
    viewer.setOrientation(QmlApplicationViewer::ScreenOrientationAuto);
    viewer.setMainQmlFile(QLatin1String("qml/UI/CaptainAdhocUI.qml"));
    viewer.showExpanded();

    QGraphicsObject * qmlRootObject = viewer.rootObject();

    GestionClients gestionClient(0);

//    QObject::connect( &gestionClient,
//                      SIGNAL( ClientDownloadSpeedUpdate( Client*,int ) ),
//                      )

    return app->exec();
}
