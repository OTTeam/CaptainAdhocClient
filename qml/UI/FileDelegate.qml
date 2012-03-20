import QtQuick 1.0

Component {
    id: listDelegate
    Item {
        id: wrapper;
        width: wrapper.ListView.view.width;
        height: if(txt.height > 75)
                    {txt.height+10}
                else{75} //50+5+5

        Item {
            id: moveMe;
            height: parent.height

            Rectangle {
                id: box
                color: "black";
                opacity: wrapper.ListView.index % 2 ? 0.2 : 0.3;
                height: wrapper.height-2;
                width: wrapper.width;
                y: 1

                Rectangle {
                        id : hoverArea
                        width: parent.width
                        height: parent.height
                        color: "lightgrey"
                        opacity: 0.7

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true
                            acceptedButtons: Qt.LeftButton | Qt.RightButton

                            onEntered:  { parent.opacity = 0.5 }
                            onExited: { parent.opacity = 0.7}
//                            onClicked: box.showInfo('Clicked (x=' + mouse.x + ' y=' + mouse.y + ' wasHeld=' + mouse.wasHeld + ')')
//                            onDoubleClicked: box.showInfo('Double clicked (x=' + mouse.x + ' y=' + mouse.y + ')')
//                            onPressAndHold: box.showInfo('Press and hold')
//                            onEntered: box.showInfo('Entered (pressed=' + pressed + ')')
//                            onExited: box.showInfo('Exited (pressed=' + pressed + ')')
                        }
                }
            }

            Image {
                id: imageTypeFile;
                x:10
                source: {
                    if (type === "avi")
                        return "images/film.png";
                    else if (type === "doc" || type === "docx")
                        return "images/doc.png"
                    else if (type === "pdf")
                        return "images/pdf.png"
                    else if (type === "jpg" || type === "jpeg" || type === "png")
                        return "images/photo.png"
                    else if (type === "mp3")
                        return "images/music.png"
                    else if (type === "rar")
                        return "images/rar.png"
                    else
                        return "images/incon.png"
                }
                width:48; height:48;
                anchors.verticalCenter: parent.verticalCenter
            }


            Item {
                id:txt
                y:10
                x:75
                Column {

                            Text {
                                    text: '<b>File Name:</b> ' + fileName
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Author :</b> ' + "pas de nom d'auteur"
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Size :</b> ' + size
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Type :</b> ' + type
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                }
            }

            Image {
                id: imDownload;
                x: 600
                source: {
                        return "images/down_arrow.png"
                }
                width:24; height:24;
                anchors.verticalCenter: parent.verticalCenter

                MouseArea{
                    id : imageMouseArea

                    hoverEnabled: true

                    anchors.fill: parent

                    onEntered: { parent.source = "images/down_arrow_clicked.png" }
                    onExited: { parent.source = "images/down_arrow.png"}
                    onClicked: {
                        console.log( "File " + model.modelData.name + " selected for download" );
                        mainUI.pickFileToDownload( model.modelData.clientFather, model.modelData.hash );
                    }
                }
            }
        }
    }
}
