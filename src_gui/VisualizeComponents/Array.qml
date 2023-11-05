import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

Repeater {

    property string myArr : "" //replace this value in create method

    model: myArr.length

    Rectangle {

        width: 50
        height: 50

        Text {
            anchors.centerIn: parent
            text: parent.parent.myArr.charAt(index)
            color: "black"
        }

        border.width: 1
        color: "white"
    }
}