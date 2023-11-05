import QtQuick
import QtQuick.Controls 2.15
import QtQuick.Layouts 6.3

import "VisualizeComponents"

Page {
    width: root.width
    height: root.height
    visible: true

    background: Rectangle {
        color: "white"
    }

    Text{
        id: stageText
        anchors.horizontalCenter: parent.horizontalCenter
        topPadding: root.height / 4
        font.pixelSize: 26
        color: "black"
    }

    ColumnLayout {

        width: parent.width
        height: parent.height

        scale: root.width / 1000.0
        spacing: 0

        ColumnLayout{
            id: visualizeBase

            Layout.alignment: Qt.AlignCenter
            spacing: 10
        }
    }

    Connections{

        target: hammingCode

        property var arrays : []

        function onPushArray(str){
            var component = Qt.createComponent("VisualizeComponents/ArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {myArr: str}));
        }

        function onPushEmptyArray(size){
            var component = Qt.createComponent("VisualizeComponents/EmptyArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {model: size}));
        }

        function onPopArray(){
            if(arrays.length > 0){
                arrays[arrays.length - 1].destroy();
            }
        }

        function onDeleteArrayAtIndex(index){
            if(arrays.length > index){
                arrays[index].destroy();
            }
        }

        function onSetBit(arrIndex, index, bit){
            arrays[arrIndex].array[index].children[0].text = bit
        }

        function onNegateBit(arrIndex, index){
            var bit = arrays[arrIndex].array[index].children[0].text;

            if(bit === "0") bit = "1";
            else bit = "0";

            arrays[arrIndex].array[index].children[0].text = bit;
        }

        function onTurnBitOff(arrIndex, index){
            arrays[arrIndex].array[index].color = "white";
        }

        function onTurnBitOn(arrIndex, index){
            arrays[arrIndex].array[index].color = "red";
        }

        Component.onCompleted: {
            stageText.text = "Encoding";
            hammingCode.encodeData(true);
        }
    }

}
