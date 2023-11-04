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

    RotationAnimator {
        id: rotationAnimator
        from: 0;
        to: 360;
        duration: 1000
        loops: Animation.Infinite
    }

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
                        function onClicked(){ Qt.quit(); }
                    }
                }

                Image {
                    Layout.preferredWidth: parent.width / 5
                    Layout.preferredHeight: Layout.preferredWidth

                    id: options

                    source: "../assets/options.png"
                    fillMode: Image.PreserveAspectFit

                    MouseArea{
                        anchors.fill: parent
                        hoverEnabled: true

                        onEntered:{
                            rotationAnimator.target = parent;
                            rotationAnimator.running = true;
                        }

                        onExited:{
                            rotationAnimator.running = false;
                        }

                        onClicked:{
                            stackView.push("Options.qml");
                        }
                    }
                }
            }

        }

        ColumnLayout{

            height: root.height
            width: root.width / 4 * 3

            Image {
                id: image

                Layout.alignment: Qt.AlignCenter
                source: "../assets/Hamming.png"
                fillMode: Image.PreserveAspectFit
            }

            Rectangle {

                Layout.alignment: Qt.AlignCenter
                width: parent.width
                height: parent.height

                Text {
                    id: descriptionText

                    text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                    font.pixelSize: 24

                    wrapMode: Text.WordWrap

                    width: parent.width //without this it won't wrap LOL

                    padding: 10
                }
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
            case 0: pageSelected = "inputHamming.qml"; break;
        }

        stackView.push(pageSelected);
    }

    function switchContent(index) {

        var imgDescAlgo = []

        switch (index) {

            case 0: {
                imgDescAlgo = ["Opis Hamminga", "../assets/Hamming.png", "Hamming"];
                break;
            }
            case 1: {
                imgDescAlgo = ["Opis drugiego", "../assets/Hamming.png", "Drugi"];
                break;
            }
            case 2:{
                imgDescAlgo = ["Opis trzeciego", "../assets/Hamming.png", "Trzeci"];
                break;
            }
        }

        descriptionText.text = imgDescAlgo[0];
        image.source = imgDescAlgo[1];
        algorithmText.text = imgDescAlgo[2];
    }

}
