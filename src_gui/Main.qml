import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts 6.3
import QtQuick

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
                    onMoved: {
                        switchContent(slider.value);
                    }
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
                        function onClicked(){ Qt.quit();}
                    }
                }

            }

        }

        ColumnLayout{

            height: root.height
            width: root.width / 4 * 3

            Image {
                id: image
                width: 300
                height: 300
                source: "../assets/test.png"
                fillMode: Image.PreserveAspectFit
                Layout.alignment: Qt.AlignCenter
            }

            Text {
                id: descriptionText
                width: 300
                height: 300
                text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                font.pixelSize: 24

                wrapMode: Text.Wrap
                padding: 10
                Layout.preferredWidth: parent.width
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

    function switchContent(index) {
        var imageSelected, descriptionSelected, algorithmSelected
        switch (index) {
            case 0: {
                descriptionSelected = "Opis Hamminga";
                imageSelected = "../assets/Hamming.png";
                algorithmSelected = "Hamming";
                break;
            }
            case 1: {
                descriptionSelected = "Opis pierwszy";
                imageSelected = "../assets/Hamming.png";
                algorithmSelected = "Pierwszy";
                break;
            }
            case 2:{
                descriptionSelected = "Opis drugi";
                imageSelected = "../assets/Hamming.png";
                algorithmSelected = "Drugi";
                break;
            }
        }
        descriptionText.text = descriptionSelected;
        image.source = imageSelected;
        algorithmText.text = algorithmSelected;
    }

}
