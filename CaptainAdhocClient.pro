# Add more folders to ship with the application, here
folder_01.source = qml/UI
folder_01.target = qml
DEPLOYMENTFOLDERS = folder_01

# Additional import path used to resolve QML modules in Creator's code model
QML_IMPORT_PATH =

symbian:TARGET.UID3 = 0xE7E95C87

# Smart Installer package's UID
# This UID is from the protected range and therefore the package will
# fail to install if self-signed. By default qmake uses the unprotected
# range value if unprotected UID is defined for the application and
# 0x2002CCCF value if protected UID is given to the application
#symbian:DEPLOYMENT.installer_header = 0x2002CCCF

# Allow network access on Symbian
symbian:TARGET.CAPABILITY += NetworkServices

# If your application uses the Qt Mobility libraries, uncomment the following
# lines and add the respective components to the MOBILITY variable.
# CONFIG += mobility
# MOBILITY +=

# Speed up launching on MeeGo/Harmattan when using applauncherd daemon
# CONFIG += qdeclarative-boostable

# Add dependency to Symbian components
# CONFIG += qt-components

# The .cpp file which was generated for your project. Feel free to hack it.
SOURCES += main.cpp \
    SimsProtocole/ServeurTCP.cpp \
    SimsProtocole/GestionClients.cpp \
    SimsProtocole/Client.cpp \
    SimsProtocole/ClientDiscovery.cpp \
    SimsProtocole/SocketHandler.cpp \
    SimsProtocole/FileStreamer.cpp \
    FileReceivedModel.cpp \
    downloadlistmodel.cpp \
    sharedfolderslistmodel.cpp \
    downloadfoldermodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += SimsProtocole/ServeurTCP.h \
    SimsProtocole/utils.h \
    SimsProtocole/GestionClients.h \
    SimsProtocole/Client.h \
    SimsProtocole/ClientDiscovery.h \
    SimsProtocole/SocketHandler.h \
    SimsProtocole/FileStreamer.h \
    FileReceivedModel.h \
    downloadlistmodel.h \
    sharedfolderslistmodel.h \
    downloadfoldermodel.h

QT += network
