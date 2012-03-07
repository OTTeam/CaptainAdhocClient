// import QtQuick 1.0 // to target S60 5th Edition or Maemo 5
import QtQuick 1.1

Item{
    width: 120
    height: 40

    BorderImage {
        id: footerBorder
        source: "images/background.png"
        width: parent.width
        height: parent.height
        border { left: 4; top: 4; right: 4; bottom: 4 }
    }

    TextInput{
        id: newDirInput
        anchors{
            margins: 4

            left: parent.left
            right: parent.right

            verticalCenter: parent.verticalCenter
        }

        font.pixelSize: parent.height - 8

        selectByMouse: true
    }
}
