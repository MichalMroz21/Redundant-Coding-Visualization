import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

Button {
    Layout.alignment: Qt.AlignHCenter

    id: backButton
    text: qsTr("Main Menu")

    Layout.margins: root.height / 20

    onClicked:{
        stackView.clear(); //will go back to Main.qml, dont do push(main.qml)
    }
}
