import QtQuick 1.1

Rectangle {
    id: simpleButton

    signal clicked

    width: 100
    height: 62

    property string label: "OK"

    radius: 5

    color: "lightgray"

    MouseArea{
        id: buttonMouseArea
        anchors.fill: parent
        hoverEnabled: true

        onClicked: simpleButton.clicked()
    }

    Text {
        id: buttonLabel
        text: parent.label

        anchors{
            horizontalCenter: parent.horizontalCenter
            verticalCenter: parent.verticalCenter
        }
    }

    Rectangle{
        id: bumpMap

        anchors.fill: parent
        radius: parent.radius

        gradient: Gradient{
            GradientStop{ position: 0.0 ; color: "transparent" }
            GradientStop{ position: 0.7 ; color: "transparent" }
            GradientStop{ position: 1.0 ; color: "#99FFFFFF" }
        }
    }

    Rectangle{
        id: pressedBumpMap

        opacity: 0.0

        anchors.fill: parent
        radius: parent.radius

        gradient: Gradient{
            GradientStop{ position: 0.0 ; color: "#99FFFFFF" }
            GradientStop{ position: 0.3 ; color: "transparent" }
            GradientStop{ position: 1.0 ; color: "transparent" }
        }
    }

    states:[
        State{
            name : "pressed"
            when : buttonMouseArea.pressed === true
            PropertyChanges { target: bumpMap ; opacity: 0.0 }
            PropertyChanges { target: pressedBumpMap ; opacity: 1.0 }
        },
        State{
            name : "disabled"
            when : enabled === false
            PropertyChanges {}
        }
    ]
}
