import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

Page {
    width: root.width
    height: root.height
    visible: true
    id: options

    ButtonGroup {
        id: selectLangGroup
    }

    background: Rectangle {
        color: "white"
    }

    ColumnLayout{

        width: options.width
        height: options.height
        spacing: 20

        ColumnLayout{
            Layout.alignment: Qt.AlignCenter

            Text{
                id: languageText
                Layout.alignment: Qt.AlignHCenter
                text: qsTr("Wybierz język / Choose language")
                font.pixelSize: 20

                wrapMode: Text.WordWrap

                width: parent.width //without this it won't wrap LOL

                padding: 10
            }
            RowLayout{

                Layout.alignment: Qt.AlignCenter

                ColumnLayout{
                    Layout.alignment: Qt.AlignCenter

                    Image{
                        id: plIcon
                        source: "../assets/pl.png"
                    }
                    RadioButton{
                        Layout.alignment: Qt.AlignCenter
                        id: plRadioButton
                        ButtonGroup.group: selectLangGroup
                    }
                }
                ColumnLayout{
                    Image {
                        id: enIcon
                        source: "../assets/en.png"
                    }
                    RadioButton{
                        Layout.alignment: Qt.AlignCenter
                        id: enRadioButton
                        ButtonGroup.group: selectLangGroup
                    }
                }
            }


            Button {
                id: backButton
                Layout.alignment: Qt.AlignHCenter
                ToolTip.delay: 1000
                ToolTip.visible: hovered
                ToolTip.text: qsTr("Wróć do menu głównego")
                text: qsTr("Wróć")

                onClicked:{
                    stackView.clear(); //will go back to Main.qml, dont do push(main.qml)
                }
            }
        }
    }

}
