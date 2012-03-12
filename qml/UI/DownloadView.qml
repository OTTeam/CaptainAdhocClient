import QtQuick 1.0

Item {
    id: screen;
    width: 640;
    height: 400

    Component.onCompleted: console.log( theList.count + " items" )

    Rectangle {
        id: background
        anchors.fill: parent;
        color: "#343434";

        Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.3 }

        DownloadDelegate {
            id: listDelegate
        }

        ListView {
            id: theList
            anchors.fill: parent
            model: mainUI.downloadsList
            delegate: listDelegate
            focus: true
            clip: true
        }
    }
}
