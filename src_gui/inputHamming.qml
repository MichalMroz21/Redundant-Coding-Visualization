import QtQuick
import QtQuick.Controls 6.3
import QtQuick.Layouts 6.3
import QtQuick 2.15

import "FormComponents"

Page {
    width: root.width
    height: root.height
    visible: true

    Connections{
        target: hammingCode
    }

    background: Rectangle {
        color: "white"
    }

    ColumnLayout{

        scale: (root.width + root.height) / 1500.0

        width: root.width
        height: root.height

        ColumnLayout {

            Layout.alignment: Qt.AlignCenter
            spacing: 20

            Text {
                Layout.alignment: Qt.AlignHCenter

                id: selectedAlgorithmText
                text: "Startowe dane"
                font.pixelSize: 32
            }

            TextField {

                id: hammingData
                color: "black"

                property int maxLength : 250;

                Layout.alignment: Qt.AlignHCenter

                background: Item {
                    implicitWidth: 0.4 * root.width
                    implicitHeight: 0.09 * root.height

                    Rectangle {
                        color: "black"
                        height: 1
                        width: parent.width
                        anchors.bottom: parent.bottom
                    }
                }

                validator: RegularExpressionValidator {regularExpression: /^[0-1]+$/}

                onTextChanged: if(length > maxLength) remove(maxLength, length);

                font.pointSize: 0.01 * (root.width + root.height)
            }

            RowLayout{

                Layout.alignment: Qt.AlignHCenter

                VisualizeCheckbox{

                    Layout.alignment: Qt.AlignRight
                    text: "<font color=\"black\">Dodatkowy bit parity</font>" //yup, the only way to change a color
                    id: additionalBit
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter

                id: vizualizeButton
                text: qsTr("Wizualizuj")

                onClicked:{

                    if(hammingData.text.length > 0){
                        hammingCode.setInitialData(hammingData.text, additionalBit.state);
                        stackView.push("Hamming.qml");
                    }
                }
            }

            Button {
                Layout.alignment: Qt.AlignHCenter

                id: backButton
                text: qsTr("Cofnij")

                onClicked:{
                    stackView.push("Main.qml");
                }
            }
        }
    }
}
