import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

RowLayout{

    Layout.alignment: Qt.AlignCenter
    spacing: 0

    property int model: 0; //replace this value in create method
    property var array : children

    Repeater {

        model: parent.model

        Rectangle {

            width: 50
            height: 50

            Text {
                anchors.centerIn: parent
                text: ""
                color: "black"
            }

            border.width: 1
            color: "white"
        }
    }
}


