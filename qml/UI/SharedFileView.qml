import QtQuick 1.1

/* Implement a list displaying available files or shared folders.
 * Display an icon (either depending on file's type, or a folder icon) and a
 * name or a path.
 *
 * Quoting the relevant help section, a ListView must be
 * provided with a list model holding the data to display and a delegate telling
 * the list widget how to display everything.
 *
 * The list model can be of various origin, but the one that we are going to use
 * will probably be a custom C++ class inheriting from QAbstractListModel.
 */

Rectangle{
    id : genericList

    // default size of the list view
    width : 200
    height : 200

    property int listWidth: width

    property int itemHeight: 60

    Component{
        id : sharedFileDelegate

        // display basic informations : icon and name or path.
        Rectangle{
            width: listWidth
            height: ( itemHeight > 30 ? itemHeight : 30 )

            gradient: Gradient{
                GradientStop{ position: 0.0 ; color: "transparent" }
                GradientStop{ position: 0.9 ; color: "transparent" }
                GradientStop{ position: 1.0 ; color: "#80FFFFFF" } // half transparent white
            }

            Grid{
                columns: 2
                rows: 1

                spacing: 10

                anchors.fill: parent

                Image{
                    id: itemIcon
                    //source: "whatever" - TODO

                    width: itemHeight - 20
                    height: itemHeight - 20
                }

                Grid{
                    columns: 1
                    rows: 2

                    spacing: 5

                    anchors.fill: parent


                    Text {
                        id: itemName
                        text: "<b>Name :</b> whatever" // - TODO
                    }


                }
            }
        }
    }

    Component{
        id : itemHighlight

        Rectangle {
            // should not be necessary if set by the delegate
            //width: 180
            //height: 40

            color: "lightsteelblue"
            radius: 5

            y: theList.currentItem.y

            Behavior on y {
                SpringAnimation {
                    spring: 3
                    damping: 0.2
                }
            }
        }
    }

    ListView{
        id: theList

        anchors.fill: parent

        delegate: sharedFileDelegate
        highlight: itemHighlight

        focus : true
    }
}
