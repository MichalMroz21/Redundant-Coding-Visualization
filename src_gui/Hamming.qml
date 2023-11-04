import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

Page {
    width: root.width
    height: root.height
    visible: true

    Connections{
        target: hammingCode

        //put signals from c++ here that change visual stuff and shit
    }

    background: Rectangle {
        color: "white"
    }

    RowLayout {

        width: parent.width
        height: parent.height

        scale: root.width / 1000.0

        RowLayout{

            Layout.alignment: Qt.AlignCenter
            spacing: 0

            id: visualizeRow

            property string myArr : hammingCode.getDataStr();

            Repeater {

                model: visualizeRow.myArr.length

                Rectangle {

                    width: 50
                    height: 50

                    Text {
                        anchors.centerIn: parent
                        text: visualizeRow.myArr.charAt(index)
                        color: "black"
                    }

                    border.width: 1
                    color: "white"
                }
            }

            Component.onCompleted: {
                hammingCode.encodeData(true);
            }
        }
    }

}
