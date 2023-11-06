import QtQuick
import QtQuick.Controls 6.3
import QtQuick.Layouts 6.3
import QtQuick 2.15

ColumnLayout{

    Layout.topMargin: root.height / 8

    scale: (root.width + root.height) / 1500.0

    width: root.width
    height: root.height / 4

    Layout.alignment: Qt.AlignCenter

    property var visualizeConnection;

    ColumnLayout {

        Layout.alignment: Qt.AlignCenter
        spacing: 20

        Text {
            Layout.alignment: Qt.AlignHCenter

            text: "Poprawianie błędów"
            font.pixelSize: 20
            color: "black"
        }

        Text{
            Layout.alignment: Qt.AlignHCenter

            text: "Zmień od 0 do 2 bitów klikając na nie"
            font.pixelSize: 15
            font.italic: true
            color: "black"
        }

        Button {
            Layout.alignment: Qt.AlignHCenter

            text: qsTr("Zacznij")

            onClicked:{
                visualizeConnection.correctingErrors();
            }
        }
    }
}

