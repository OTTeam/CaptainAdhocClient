import QtQuick 1.1

Rectangle {
    id: mainUI

    property string downloadFolder: "/C/Users/Public/Downloads"
    property bool connected : true;

    signal pickDownloadFolder()
    signal pickSharedDir()
    signal delSharedDir( int index )
    signal pickFileToDownload( int pointer, string fileHash )
    signal requestConnection()
    signal requestDisconnection()

    function connectionDone(){
        console.log( "[UI] RECEIVED connectionDone signal" )
        deconnexion.enabled = true;
        deconnexion.text = "Disconnection";
        connected = true;
    }

    function disconnectionDone(){
        console.log( "[UI] RECEIVED disconnectionDone signal" )
        connected = false
        deconnexion.enabled = true;
        deconnexion.text = "Connection"
        hostsNumber.hosts = 0;
    }

    function updateHostsNumber( newNumber ){
        hostsNumber.hosts = newNumber;
    }

    width: 640
    height: 550

    property int ribbonHeight: 90
    property int separatorHeight : 1

    Component.onCompleted: {
        buttonPrevious.clicked.connect( mainDisplay.gotoPrevSlide )
        buttonNext.clicked.connect( mainDisplay.gotoNextSlide )

        mainDisplay.slideChanged.connect( updateLabels )
    }

    // ribbon
    Rectangle{
        id:ribbon
        Image { source: "images/bg.png";
                height: 90
                width: 640
        }

        SearchBox {
            id: search;
            focus: true

            anchors {
                bottom: parent.bottom;
                horizontalCenter: parent.horizontalCenter
                bottomMargin: 5
            }
        }

        height: ribbonHeight

        anchors{
            left: parent.left
            right: parent.right
        }
    }

    // taskbar
    Rectangle{
        id:taskbar
        Image {
                id: imTaskbar
                source: "images/bg.png";
                height: 30
                width: 640
        }

        height: 30
        width: 640

        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }


        Item {
            id: hostsNumber
            property int hosts: 0
            Text {
                    text: " Connected users : " + hostsNumber.hosts
                    color: "#999999";
                    style: Text.Raised;
                    styleColor: "black";
                    font.pointSize: 11
                    anchors.verticalCenter: parent.verticalCenter
            }
            anchors{
                left: parent.left
                verticalCenter: parent.verticalCenter
            }
        }

        Item {
            id: userProfil
            Text {
                text: "Welcome, " + "<b>Captain</b>"
                color: "#999999";
                style: Text.Raised;
                styleColor: "black";
                font.pointSize: 11
                anchors{
                    centerIn: parent
                }
            }
            height: 30
            anchors{
                centerIn: parent
            }
        }

        Button{
            id : deconnexion
            text: "Disconnection"
            width: 100
            height: 30
            anchors{
                right: parent.right
                bottom: parent.bottom
            }
            onClicked: {
                console.log( "Button clicked with 'connected' being " + connected );
                if( connected ){
                    console.log( "[UI] requesting disconnection" );
                    mainUI.requestDisconnection();
                    text = "Disconnecting...";
                }
                else
                {
                    console.log( "[UI] requesting connection" );
                    mainUI.requestConnection();
                    text = "Connecting..."
                }
                enabled = false
            }
        }
    }

    // separator
    Rectangle{
        id: separator
        gradient: Gradient{
            GradientStop{ position: 0.0; color: "lightgrey" }
            GradientStop{ position: 0.5; color: "#1A1A1A" }
            GradientStop{ position: 1.0; color: "lightgrey" }
        }

        anchors{
            top: ribbon.bottom
            left: parent.left
            right: parent.right
        }

        height: separatorHeight
    }

    SlideWidget{
        id: mainDisplay

        anchors{
            top: separator.bottom
            left: parent.left
            right: parent.right
            bottom: taskbar.top
        }

        current: 0

        // configuration slide
        Rectangle{
            id: configurationSlide
            color: "black";
            opacity : 0.7
            Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.3 }

            property string title: "  Shared Folder  "

            width: parent.width
            height: parent.height

            SharedDirectoriesView{
                id: sharedDirView

                anchors{
                    top: parent.top
                    left: parent.left
                    right: parent.right
                    bottom: downloadFolderConfig.top
                }
            }

            Item{
                id: downloadFolderConfig
                height: 64

                anchors{
                    margins: 10
                    left: parent.left
                    right: parent.right
                    bottom: parent.bottom
                }

                ImageButton{
                    id : buttonAdd
                    textColor: "white"

                    imgSource: "images/tools.png"
                    imgHooverSource : "images/tools_hover.png"
                    imgClickedSource : "images/tools_clicked.png"
                    imgWidth: 64
                    imgHeight: 64

                    onClicked: { mainUI.pickDownloadFolder() }
                }

                Text {
                    Component.onCompleted: console.log( width )
                    id: label
                    text: "<b>Download folder : </b>"
                    textFormat: Qt.RichText
                    font.pointSize: 9
                    color: "#cccccc";
                    style: Text.Raised;
                    styleColor: "black";
                    width: paintedWidth
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: buttonAdd.right
                        leftMargin: 20
                    }
                    font.pixelSize: 16
                }
                Text{
                    id: daFolder
                    text : downloadFolder
                    elide: Text.ElideMiddle
                    font : label.font
                    color : label.color
                    style: label.style
                    styleColor: label.styleColor
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: label.right
                        right: parent.right
                        margins: 10
                    }
                }
            }
        }

        // downloads
        Rectangle{
            id: downloads
            color:"darkgray"

            property string title: "  Downloads  "

            width: parent.width
            height: parent.height

            DownloadView{
                id: downloadFileView
                anchors.fill: parent
            }
        }

        // available files
        Rectangle{
            id: availableFilesView
            color:"darkgray"

            property string title: "  Home  "

            width: parent.width
            height: parent.height

            FileView{
                id: availableFileView
                anchors.fill: parent
            }
        }
    }

    ImageButton{
        id : buttonPrevious
        textColor: "white"

        imgSource: "images/left_arrow.png"
        imgHooverSource : "images/left_arrow_hover.png"
        imgClickedSource : "images/left_arrow_clicked.png"
        imgWidth: 48
        imgHeight: 48
        label : "previous Slide"
        labelPosition : "right"

        anchors{
            left: parent.left
            top: parent.top
            margins: 20
        }
    }

    ImageButton{
        id : buttonNext
        textColor: "white"

        imgSource: "images/right_arrow.png"
        imgHooverSource: "images/right_arrow_hover.png"
        imgClickedSource: "images/right_arrow_cliked.png"
        imgWidth: 48
        imgHeight: 48

        label: "next slide"
        labelPosition: "left"

        anchors{
            right: parent.right
            top: parent.top
            margins: 20
        }
    }

    Text{
        id : currSlideTitle

        color: "#999999";
        style: Text.Raised;
        styleColor: "black";

        anchors{
            verticalCenter: buttonPrevious.verticalCenter
            horizontalCenter: parent.horizontalCenter
        }

        text: ""

        font{
            bold: false
            pointSize: 20
        }
    }

    function updateLabels(){
        currSlideTitle.text  = mainDisplay.getCurrentSlideTitle();
        buttonPrevious.label = mainDisplay.getPrevSlideTitle();
        buttonNext.label     = mainDisplay.getNextSlideTitle();
    }
}
