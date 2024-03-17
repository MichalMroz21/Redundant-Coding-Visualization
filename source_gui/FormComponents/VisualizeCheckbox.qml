import QtQuick 2.12
import QtQuick.Controls 2.12


CheckBox{

    indicator: Rectangle {
        implicitWidth: 20
        implicitHeight: 20
        x: parent.leftPadding
        y: parent.height / 2 - height / 2
        radius: 3
        border.color: parent.down ? "#17a81a" : "#21be2b"

        Text {
            width: 10
            height: 10
            x: 1
            y: -3
            text: "✔"
            font.pointSize: 18
            color: parent.parent.down ? "#17a81a" : "#21be2b"
            visible: parent.parent.checked
        }
    }
}
