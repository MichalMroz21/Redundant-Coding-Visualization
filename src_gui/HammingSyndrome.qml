import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

import "VisualizeComponents"

Page {
    width: root.width
    height: root.height
    visible: true

    background: Rectangle {
        color: "white"
    }
	
    ColumnLayout {

        id: columnBase

        width: parent.width
        height: parent.height

        anchors.horizontalCenter: parent.horizontalCenter

        spacing: 0

        Layout.alignment: Qt.AlignCenter

        Text{
            font.pixelSize: 20
            color: "black"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Otrzymana wiadomość")
        }

        Column {
            id: receivedColumn
            Layout.alignment: Qt.AlignHCenter

        }

        Text {
            font.pixelSize: 20
            color: "black"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Syndrom błędu")
        }

        Column {
            id: syndromColumn
            Layout.alignment: Qt.AlignHCenter

        }

        Text{
            font.pixelSize: 20
            color: "black"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Wektor błędów")
        }

        Column {
            id: errorColumn
            Layout.alignment: Qt.AlignHCenter

        }

        Text{
            font.pixelSize: 20
            color: "black"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Uzyskana wiadomość; Otrzymana wiadomość XOR Wektor błędów")
        }

        Column {
            id: encodedColumn
            Layout.alignment: Qt.AlignHCenter
        }

        Text{
            font.pixelSize: 20
            color: "black"
            Layout.alignment: Qt.AlignHCenter
            text: qsTr("Odkodowana wiadomość")
        }

        Column {
            id: endColumn
            Layout.alignment: Qt.AlignHCenter
        }

        Button {
            Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

            text: qsTr("Main Menu")

            onClicked: {
                stackView.clear();
            }
        }

    }

    Connections{

        target: hammingCode

        id: hammingVisualizeConnection

        Component.onCompleted: {
            let component = Qt.createComponent("VisualizeComponents/ArrayRowLayout.qml");

            let syndrom = hammingCode.getSyndrome();
            component.createObject(syndromColumn, {myArr: syndrom, isExtended: hammingCode.getEncodingExtended()});


            let error = hammingCode.getError();
            component.createObject(errorColumn, {myArr: error, isExtended: hammingCode.getEncodingExtended()});

            let encoded = hammingCode.getEncodedStr();
            component.createObject(encodedColumn, {myArr: encoded, isExtended: hammingCode.getEncodingExtended()});

            let received = hammingCode.getReceivedCode();
            component.createObject(receivedColumn, {myArr: received, isExtended: hammingCode.getEncodingExtended()});

            let decoded = hammingCode.getDecodedStr();
            component.createObject(endColumn, {myArr: decoded, isExtended: hammingCode.getEncodingExtended()});
        }

    }
}
