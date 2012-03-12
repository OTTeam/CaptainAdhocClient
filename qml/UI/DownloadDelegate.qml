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
                    if (model.modelData.type === "avi")
                        return "images/film.png";
                    else if (model.modelData.type === "doc" || model.modelData.type === "docx")
                        return "images/doc.png"
                    else if (model.modelData.type === "pdf")
                        return "images/pdf.png"
                    else if (model.modelData.type === "jpg" || model.modelData.type === "jpeg" || model.modelData.type === "png")
                        return "images/photo.png"
                    else if (model.modelData.type === "mp3")
                        return "images/music.png"
                    else if (model.modelData.type === "rar")
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
                                    text: '<b>File Name:</b> ' + model.modelData.name
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Author :</b> ' + authorName
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Size :</b> ' + model.modelData.size
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                            Text {
                                    text: '<b>Type :</b> ' + model.modelData.type
                                    textFormat: Qt.RichText
                                    font.pointSize: 9
                                    color: "#cccccc";
                                    style: Text.Raised;
                                    styleColor: "black";
                                    wrapMode: Text.WordWrap
                            }
                }
            }

            Item{
                id : status
                visible: ( model.modelData.progress >= 0 && model.modelData.progress <= 100 )
                ProgressBar{
                    id: downBar
                    x: 300
                    y: 28
                    value: model.modelData.progress
                }
            }


            ImageButton{
                id: delButton

                visible: ( model.modelData.progress >= 0 && model.modelData.progress <= 100 )

                imgSource: "images/delete.png"
                imgHooverSource: "images/delete_hover.png"
                imgClickedSource: "images/delete_clicked.png"
                imgHeight: parent.height - 5
                imgWidth: imgHeight

                x: parent.width - imgHeight - 3.5
                y: 2.5
            }
/*
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
                }
            }
            */
        }
    }
}
