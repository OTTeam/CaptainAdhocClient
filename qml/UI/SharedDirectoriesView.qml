import QtQuick 1.1

Item {
    id: sharedDirectoriesView

    width: 150
    height: 300

    Component {
        id: dirDelegate

        Item {
            Component.onCompleted: {
                console.log( width + " // " + folder.width + " // " + ListView.isCurrentItem )
            }

            width: parent.width
            height: folder.height

            Rectangle {
                    id : hoverArea
                    width: parent.width
                    height: parent.height
                    color: "black"
                    opacity: 0.1

                    MouseArea {
                        anchors.fill: parent
                        hoverEnabled: true
                        onEntered:  { parent.opacity = 0.5 }
                        onExited: { parent.opacity = 0.1}
                   }
            }


//            MouseArea{
//                anchors.fill: parent
//                //onClicked: dirDelegate.state = "selected"
//            }

            Text {
                id: folder
                text: folderPath
                textFormat: Qt.RichText
                color: "#cccccc";
                style: Text.Raised;

                anchors{
                    margins: 2
                    left: parent.left
                    right: delButton.left
                }

                elide: Text.ElideRight

                font{
                    pointSize: 12
                }
            }

            ImageButton{
                id: delButton

                imgSource: "images/delete.png"
                imgHooverSource: "images/delete_hover.png"
                imgClickedSource: "images/delete_clicked.png"
                imgHeight: parent.height - 5
                imgWidth: imgHeight

                x: parent.width - imgHeight - 3.5
                y: 2.5
            }
        }
    }

//    Component{
//        id: dirHighlight

//        Rectangle {
//            color: "lightsteelblue"

//            border{
//                color: "steelblue"
//                width: 1
//            }

//            radius: 3

//            y: theDirView.currentItem.y

//            Behavior on y {
//                SmoothedAnimation { velocity: 200 }
//            }
//        }
//    }

    ListView{
        id: theDirView

        anchors.fill: parent

        //highlight: dirHighlight
        delegate: dirDelegate

        model: DirModel{}

        footer: Item {
            id: listFooter
            width: parent.width
            height: 100

            ImageButton{
                id : buttonAdd
                textColor: "white"

                imgSource: "images/plus.png"
                imgHooverSource : "images/plus_hover.png"
                imgClickedSource : "images/plus_clicked.png"
                imgWidth: 64
                imgHeight: 64

                anchors{
                    horizontalCenter: parent.horizontalCenter
                    bottom: parent.bottom
                }
            }

        }

        clip: true
        focus: true
    }
}
