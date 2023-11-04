import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts 6.3

Window {
    id: root
    width: 1280
    height: 720
    visible: true
    title: qsTr("Redundant Coding")

    Row{

        ColumnLayout{

            width: root.width / 4
            height: root.height

            Layout.alignment: Qt.AlignCenter

            ColumnLayout{

                spacing: 10

                Text {
                    id: selectedAlgorithmText
                    text: "Wybrany algorytm:"
                    font.pixelSize: 16
                }

                Text {
                    id: algorithmText
                    text: qsTr("Hamming")
                    font.pixelSize: 20
                }

                Slider {
                    id: slider
                    snapMode: RangeSlider.SnapOnRelease
                    stepSize: 1
                    to: 2
                    value: 0
                }

                Button {
                    id: confirmButton
                    text: qsTr("Zatwierdź")
                }

                Button {
                    id: exitButton
                    text: qsTr("Wyjście")
                }

            }

        }

        ColumnLayout{

            height: root.height

            Image {
                id: image
                width: 300
                height: 300
                source: "../assets/test.png"
                fillMode: Image.PreserveAspectFit
            }

            Text {
                id: descriptionText
                width: 300
                height: 300
                text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                font.pixelSize: 24

                wrapMode: Text.WrapAnywhere
            }


        }
    }
}
