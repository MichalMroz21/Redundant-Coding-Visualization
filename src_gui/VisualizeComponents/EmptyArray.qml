import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

Repeater {

    Rectangle {

        width: 50
        height: 50

        model : 0 //replace this value in create method

        Text {
            anchors.centerIn: parent
            text: " "
            color: "black"
        }

        border.width: 1
        color: "white"
    }
}
