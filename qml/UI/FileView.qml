import QtQuick 1.0

Item {
    id: screen;
    width: 640;
    height: 400

    Rectangle {
        id: background
        anchors.fill: parent;
        color: "#343434";

        Image { source: "images/stripes.png"; fillMode: Image.Tile; anchors.fill: parent; opacity: 0.3 }

        FileDelegate {
            id: listDelegate
        }

        ListView {
            anchors.fill: parent
            model: FileModel {}
            delegate: listDelegate
            focus: true
            clip: true
        }
    }
}
