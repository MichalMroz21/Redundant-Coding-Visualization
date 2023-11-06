import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

RowLayout{

    Layout.alignment: Qt.AlignCenter
    spacing: 0

    property int model: 0; //replace this value in create method
    property var array : children
    property bool clickChange : false

    property bool isExtended

    property int changeLimit : 1 + isExtended
    property int currentChanged : 0
    property var changedArr : new Array(model).fill(false);

    Repeater {

        model: parent.model

        Rectangle {

            width: 50
            height: 50

            Text {
                anchors.centerIn: parent
                text: " "
                color: "black"
            }

            border.width: 1
            color: "white"

            MouseArea{
                enabled: parent.parent.clickChange
                anchors.fill: parent
                acceptedButtons: Qt.LeftButton | Qt.RightButton

                onClicked : {
                    if(parent.parent.changedArr[index] === false){
                        if(parent.parent.currentChanged < parent.parent.changeLimit){
                            if(parent.children[0].text === "0"){
                                parent.children[0].text = "1";
                            }
                            else parent.children[0].text = "0";

                            parent.parent.currentChanged += 1;
                            parent.parent.changedArr[index] = true;
                        }
                    }

                    else{
                        parent.parent.changedArr[index] = false;
                        parent.parent.currentChanged -= 1;

                        if(parent.children[0].text === "0"){
                            parent.children[0].text = "1";
                        }
                        else parent.children[0].text = "0";
                    }
                }
            }
        }
    }
}


