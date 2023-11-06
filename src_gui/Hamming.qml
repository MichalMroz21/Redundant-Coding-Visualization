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

        id: columnBase

        width: parent.width
        height: parent.height

        scale: root.width / 1000.0
        spacing: 0

        Layout.alignment: Qt.AlignCenter

        ColumnLayout{
            id: visualizeBase

            Layout.alignment: Qt.AlignCenter
            spacing: 10
        }
    }

    Text{
        id: belowText
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottom: parent.bottom
        bottomPadding: root.height / 4
        font.pixelSize: 20
        color: "dodgerblue"
        text: ""
    }

    Item{
        id: formBase
    }

    Timer {
       id: timer
    }

    Connections{

        target: hammingCode

        id: hammingVisualizeConnection

        property var arrays : []

        function delay(delayTime, cb) {
            timer.interval = delayTime;
            timer.repeat = false;
            timer.triggered.connect(cb);
            timer.start();
        }

        function getArrayStr(arrIndex){

            var bitStr = "";

            for(var i = 0; i < arrays[arrIndex].array.length - 1; i++){
                bitStr += arrays[arrIndex].array[i].children[0].text;
            }

            return bitStr;
        }


        function onPushArray(str){
            var component = Qt.createComponent("VisualizeComponents/ArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {myArr: str, isExtended: hammingCode.getEncodingExtended()}));
        }

        function onPushEmptyArray(size){
            var component = Qt.createComponent("VisualizeComponents/EmptyArrayRowLayout.qml");
            arrays.push(component.createObject(visualizeBase, {model: size, isExtended: hammingCode.getEncodingExtended()}));
        }

        function onInsertArray(index, str){

            if(arrays.length === 0){
                onPushArray(str);
                return;
            }

            var bitStrs = [];

            for(var i = 0; i < arrays.length; i++){
                bitStrs.push(getArrayStr(i));
            }

            for(var k = 0; k < arrays.length; k++){
                onDeleteArrayAtIndex(k);
            }

            for(var j = 0; j < bitStrs.length; j++){
                if(j === index) onPushArray(str);
                onPushArray(bitStrs[j]);
            }
        }

        function onPopArray(){
            if(arrays.length > 0){
                arrays[arrays.length - 1].destroy();
                arrays.pop();
            }
        }

        function onDeleteArrayAtIndex(index){
            if(arrays.length > index){
                arrays[index].destroy();
                arrays.splice(index, 1);
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

        function onInsertBit(arrIndex, index, bit){
            var bitStr = getArrayStr(arrIndex);

            onDeleteArrayAtIndex(arrIndex);

            bitStr = bitStr.slice(0, index) + bit + bitStr.slice(index);

            onInsertArray(arrIndex, bitStr);
        }

        function onInsertEmptyBit(arrIndex, index, bit){
            var bitStr = getArrayStr(arrIndex);

            onDeleteArrayAtIndex(arrIndex);

            bitStr = bitStr.slice(0, index) + " " + bitStr.slice(index);

            onInsertArray(arrIndex, bitStr);
        }

        function onTurnBitOff(arrIndex, index){
            arrays[arrIndex].array[index].color = "white";
        }

        function onTurnBitOn(arrIndex, index, color){
            if(color === "") color = "red";
            arrays[arrIndex].array[index].color = color;
        }

        //its kinda scuffed though, so don't use it, lol
        function onTurnBitOnAutoOff(arrIndex, index, color){

            if(color === "") color = "red";

            arrays[arrIndex].array[index].color = color;

            var fn = function(){
                onTurnBitOff(arrIndex, index);
                timer.triggered.disconnect(fn);
            };

            delay(0.995 * hammingCode.getAnimationDelayMs(), fn);
        }

        function onSetBelowText(str){
            belowText.text = str;
        }

        function onSetClickAllow(arrIndex, isAllowed){
            arrays[arrIndex].clickChange = isAllowed;
        }

        Component.onCompleted: {
            stageText.text = "Encoding...";
            hammingCode.encodeData(true);
        }

        property var hammingError;

        function onEncodingEnd(){

            belowText.text = "";
            stageText.text = "";

            var component = Qt.createComponent("VisualizeComponents/correctErrorHammingForm.qml");
            hammingError = component.createObject(visualizeBase, {visualizeConnection: hammingVisualizeConnection, isExtended: hammingCode.getEncodingExtended()});

            onSetClickAllow(0, true);
        }

        function correctingErrors(){

            onSetClickAllow(0, false);

            hammingError.destroy();
            stageText.text = "Finding error(s)...";

            hammingCode.sendCode(getArrayStr(0));
            hammingCode.correctError(true);
        }
    }

}
