import QtQuick.Window
import QtQuick.Controls
import QtQuick.Layouts 6.3
import QtQuick.Dialogs
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
            id:menuSidebarColumn
            width: root.width / 4
            height: root.height

            ColumnLayout{

                id:menuColumn
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

                ComboBox {
                    id: comboBox
                    model: ListModel{
                        ListElement { text: "Hamming" }
                        ListElement { text: "Drugi" }
                        ListElement { text: "Trzeci" }
                    }
                    onActivated:{
                        switchContent(currentIndex);
                    }
                }

                Button {
                    id: confirmButton
                    text: qsTr("Zatwierdź")
                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Potwierdź wybór algorytmu")

                    onClicked: {
                        switchPage(comboBox.currentIndex);
                    }
                }

                Button {
                    id: exitButton
                    text: qsTr("Wyjście")
                    ToolTip.delay: 1000
                    ToolTip.visible: hovered
                    ToolTip.text: qsTr("Zakończ działanie programu")

                    onClicked: messageDialog.open()
                }

                MessageDialog {
                    id: messageDialog
                    title: "Zakończ"
                    text: "Czy chcesz zakończyć działanie programu?"
                    buttons: MessageDialog.Yes | MessageDialog.Cancel
                    onAccepted: {
                        console.log("End of the program.")
                        Qt.quit()
                    }
                    Component.onCompleted: visible = false
                }

                Image {
                    Layout.preferredWidth: parent.width / 5
                    Layout.preferredHeight: Layout.preferredWidth

                    id: options

                    source: "../assets/options.png"
                    fillMode: Image.PreserveAspectFit

                    MouseArea{
                        id:optionsMouseArea
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
            id:descriptiveColumn
            height: root.height
            width: root.width / 4 * 3
            spacing: 0

            Rectangle{
                id:imageRectangle

                width: parent.width
                height: parent.height*0.7
                Layout.alignment: Qt.AlignCenter

                Image {
                    id: image
                    anchors.centerIn: parent
                    Layout.alignment: Qt.AlignCenter
                    source: "../assets/Hamming.png"
                    fillMode: Image.PreserveAspectFit
                }
            }

            Rectangle {
                id:descriptionRectangle
                Layout.alignment: Qt.AlignCenter
                width: parent.width
                height: parent.height*0.3

                Text {
                    id: descriptionText

                    text: qsTr("Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.")
                    font.pixelSize: 20

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

    Component.onCompleted: {
        switchContent(0);
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
                imgDescAlgo = ["Kod Hamminga (n,k) koduje k bitów informacji na n bitach z użyciem dodatkowych bitów parzystości. Pozwala na detekcję i naprawę pojedynczego błędu.\nW tej implementacji kod jest generowany na podstawie macierzy kontroli parzystości, gdzie reprezentacja liczbowa syndromu błędu wskazuje na pozycję, na której wystąpił błąd (syndrom 0 oznacza brak błędu). Bity parzystości są umieszczone na pozycjach będących potęgami 2 (1, 2, 4...), pierwszy zawiera XORa bitów będących na pozycjach, które mają 1 na najmłodszym bicie, drugi na drugim najmłodszym bicie itp", "../assets/Hamming.png", "Hamming"];
                break;
            }
            case 1: {
                imgDescAlgo = ["Opis drugiego", "../assets/en.png", "Drugi"];
                break;
            }
            case 2:{
                imgDescAlgo = ["Opis trzeciego", "../assets/pl.png", "Trzeci"];
                break;
            }
        }

        descriptionText.text = imgDescAlgo[0];
        image.source = imgDescAlgo[1];
        algorithmText.text = imgDescAlgo[2];
    }

}
