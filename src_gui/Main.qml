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



            ColumnLayout{
                Layout.alignment: Qt.AlignCenter
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
                    onClicked: {
                        switchPage(slider.value);
                    }
                }

                Button {
                    id: exitButton
                    text: qsTr("Wyjście")
                    Connections {
                        target: exitButton
                        onClicked: Qt.quit()
                    }
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
    StackView{
           id: stackView
           anchors.fill: parent
    }
    function switchPage(index) {

            var pageSelected;

            switch (index) {
                case 0: pageSelected = "Hamming.qml"; break;
                //case 1: pageSelected = "1.qml"; break;
                //case 2: pageSelected = "2.qml"; break;
            }

            stackView.push(pageSelected);
        }

}
