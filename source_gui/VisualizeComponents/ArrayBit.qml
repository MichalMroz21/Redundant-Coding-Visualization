import QtQuick 2.15;

Rectangle
{
    width: 50;
    height: 50;
    color: "white";
    border.width: 1;

    property string bit : "";

    Text {
        anchors.centerIn: parent;
        text: bit;
        color: "black"
    }
}
