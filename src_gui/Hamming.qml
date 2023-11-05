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
        text: ""
        color: "black"
    }

    ColumnLayout {

        width: parent.width
        height: parent.height

        scale: root.width / 1000.0
        spacing: 0

        ColumnLayout{

            Layout.alignment: Qt.AlignCenter
            spacing: 10

            id: visualizeBase


        }
    }

    Connections{

        target: hammingCode

        property var arrays : []

        Component.onCompleted: {
            stageText.text = "Encoding";
            hammingCode.encodeData(true);
        }

        function onPushArray(str){
            var component = Qt.createComponent("VisualizeComponents/ArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {myArr: str}));
        }

        function onPushEmptyArray(size){
            var component = Qt.createComponent("VisualizeComponents/EmptyArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {model: size}));
        }

        function onPopArray(){
            arrays[arrays.length - 1].destroy();
        }

        function onDeleteArrayAtIndex(index){
            arrays[index].destroy();
        }

        function onTurnBitOff(index){

        }

        function onTurnBitOn(index){

        }

        //put signals from c++ here that change visual stuff and shit
    }

}
