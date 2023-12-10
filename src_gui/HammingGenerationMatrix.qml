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
            id: stageText
            topPadding: root.height / 8
            font.pixelSize: 26
            color: "black"
            Layout.alignment: Qt.AlignHCenter | Qt.AlignTop
            text: qsTr("Macierz generacyjna")
        }

        Column {
            id: matrixColumn
            Layout.alignment: Qt.AlignHCenter

        }

        Button {
            Layout.alignment: Qt.AlignHCenter

            ToolTip.delay: 1000
            ToolTip.visible: hovered
            text: qsTr("Następny krok")

            onClicked: {
                stackView.push("Hamming.qml");
            }
        }
    }

    Connections{

        target: hammingCode

        id: hammingVisualizeConnection

        Component.onCompleted: {
            let generationMatrixStr = hammingCode.getGenerationMatrixStr(true);
            let rows = generationMatrixStr.split('\n');
            for (let row of rows) {
                let component = Qt.createComponent("VisualizeComponents/ArrayRowLayout.qml");
                component.createObject(matrixColumn, {myArr: row, isExtended: hammingCode.getEncodingExtended()});
            }
        }

    }
}
