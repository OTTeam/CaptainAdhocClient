import QtQuick 1.1

// The image button is made of an image and a label. Up to three images can be
// provided, for normal, hovered and pushed states.
Rectangle {
    id: imageButton

    signal clicked

    color: "transparent"

    property string label
    property color textColor: "black"
    property real labelSize: 14
    // possible values : left, right, down
    // if none of this is used, down is used as default
    property string labelPosition

    property string imgSource
    property string imgHooverSource
    property string imgClickedSource
    property string imgDisabledSource
    property int imgWidth
    property int imgHeight

    width: childrenRect.width
    height: childrenRect.height

    onClicked: { console.log( "button clicked" ) }

    // the image button
    Image{
        id : buttonImage

        source : parent.imgSource

        width: parent.imgWidth > 0 ? parent.imgWidth : implicitWidth
        height: parent.imgHeight > 0 ? parent.imgHeight : implicitHeight

        x : ( labelPosition === "left" ? buttonLabel.width : 0 )

        MouseArea{
            id : imageMouseArea

            hoverEnabled: true

            anchors.fill: parent

            onClicked: {
                imageButton.clicked()
            }

            states : [
                State{
                    name : "pressed"
                    when : imageMouseArea.pressed == true
                    PropertyChanges { target: buttonImage; source: imgClickedSource }
                },
                State{
                    name : "hoovered"
                    when : imageMouseArea.containsMouse == true
                    PropertyChanges { target: buttonImage; source: imgHooverSource }
                },
                State{
                    name : "disabled"
                    when : imageButton.enabled == false
                    PropertyChanges { target: buttonImage; source: imgDisabledSource }
                }
            ]
        }
    }

    // this is the text label positionned next to the button
    Text{
        id: buttonLabel

        text : parent.label
        color : textColor

        x : getTextXPosition()
        y : getTextYPosition()
    }

    function getTextXPosition(){
        switch( labelPosition )
        {
        case "left":
            return 0;
        case "right":
            return buttonImage.width;
        default: // down
            return ( buttonImage.width - buttonLabel.width ) / 2;
        }
    }

    function getTextYPosition(){
        if( labelPosition == "left" || labelPosition == "right" )
        {
            return ( buttonImage.height - buttonLabel.height ) / 2;
        }
        else
        {
            return buttonImage.height;
        }
    }
}

