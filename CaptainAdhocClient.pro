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
    SimsProtocole/SocketHandler.cpp \
    SimsProtocole/GestionClients.cpp \
    SimsProtocole/ClientDiscovery.cpp \
    SimsProtocole/FileStreamer.cpp \
    SimsProtocole/Client.cpp \
    FileIndexing/FileIndexDao.cpp \
    FileIndexing/FileModel.cpp \
    FileIndexing/FileIndexer.cpp \
    FileIndexing/FileUtils.cpp \
    FileIndexing/SimpleFileModel.cpp \
    FileIndexing/FolderDao.cpp \
    FileIndexing/FolderModel.cpp \
    Wifi/ManagerNotificationSink.cpp \
    Wifi/NetworkNotificationSink.cpp \
    Wifi/SecuritySettings.cpp \
    Wifi/WifiConnection.cpp \
    Wifi/WifiInterface.cpp \
    Wifi/WifiManager.cpp \
    Wifi/WifiNetwork.cpp \
    FileReceivedModel.cpp \
    sharedfolderslistmodel.cpp \
    downloadfoldermodel.cpp \
    filelistmodel.cpp \
    availablefileslistshandler.cpp
    #availablefileslistmodel.cpp

# Please do not modify the following two lines. Required for deployment.
include(qmlapplicationviewer/qmlapplicationviewer.pri)
qtcAddDeployment()

HEADERS += SimsProtocole/ServeurTCP.h \
    SimsProtocole/GestionClients.h \
    SimsProtocole/Client.h \
    SimsProtocole/ClientDiscovery.h \
    SimsProtocole/SocketHandler.h \
    SimsProtocole/FileStreamer.h \
    SimsProtocole/utils.h \
    FileIndexing/FileIndexDao.h \
    FileIndexing/FileModel.h \
    FileIndexing/FileIndexer.h \
    FileIndexing/FileUtils.h \
    FileIndexing/SimpleFileModel.h \
    FileIndexing/FolderDao.h \
    FileIndexing/FolderModel.h \
    Wifi/ManagerNotificationSink.h \
    Wifi/NetworkNotificationSink.h \
    Wifi/SecuritySettings.h \
    Wifi/WifiConnection.h \
    Wifi/WifiInterface.h \
    Wifi/WifiManager.h \
    Wifi/WifiNetwork.h \
    FileReceivedModel.h \
    sharedfolderslistmodel.h \
    downloadfoldermodel.h \
    filelistmodel.h \
    availablefileslistshandler.h
    #availablefileslistmodel.h

QT += network sql

TEMPLATE = app
