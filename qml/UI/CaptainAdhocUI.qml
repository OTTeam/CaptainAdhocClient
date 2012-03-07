import QtQuick 1.1

Rectangle {

    width: 640
    height: 550

    property int ribbonHeight: 90
    property int separatorHeight : 1

    Component.onCompleted: {
        buttonPrevious.clicked.connect( mainDisplay.gotoPrevSlide )
        buttonNext.clicked.connect( mainDisplay.gotoNextSlide )

        mainDisplay.slideChanged.connect( updateLabels )
    }

    Rectangle{
        id:ribbon
        Image { source: "images/bg.png";
                height: 90
                width: 640
        }

        height: ribbonHeight

        anchors{
            left: parent.left
            right: parent.right
        }
    }


    Rectangle{
        id:taskbar
        Image { source: "images/bg.png";
                height: 30
                width: 640
        }

        height: 30

        anchors{
            left: parent.left
            right: parent.right
            bottom: parent.bottom
        }


        Item {
            id: vitesse
            Column {
                Text {
                    text: " Download : Kb/s "
                    color: "#999999";
                    style: Text.Raised;
                    styleColor: "black";
                }
                Text {
                    text: " Upload : Kb/s "
                    color: "#999999";
                    style: Text.Raised;
                    styleColor: "black";
                }
            }
            height: 30
            anchors{
                left: parent.left
                bottom: parent.bottom
                verticalCenter: taskbar.verticalCenter
            }
        }

        Button{
            id : deconnexion
            text: "Deconnexion"
            width: 100
            height: 30
            anchors{
                right: parent.right
                bottom: parent.bottom
            }
        }
    }

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

        current: 2

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
                width: parent.width
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

                }

                Text {
                    id: label
                    text: "<b>Download folder : </b>" + " D:/Users/Documents/SharedDownloads "
                    textFormat: Qt.RichText
                    font.pointSize: 9
                    color: "#cccccc";
                    style: Text.Raised;
                    styleColor: "black";
                    anchors{
                        verticalCenter: parent.verticalCenter
                        left: buttonAdd.right
                        leftMargin: 20
                    }
                    font.pixelSize: 16
                }
            }
        }

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

        Rectangle{
            id: thirdSlide
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
