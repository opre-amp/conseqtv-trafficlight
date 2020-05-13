import QtQuick 2.14
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.StateMachine 1.14 as DSM

import userdata 1.0

import 'RESTclient.js' as RESTClient


Image{
    id: display_img
    source: 'qrc:/fsm/Images/0.png'

    property variant lut: ["Off",
        "Blinking yellow",
        "Yellow",
        "Red & pedestrian red (1)",
        "Red & pedestrian green",
        "Red & pedestrian green & stop",
        "Red & blinking pedestrian green",
        "Red & pedestrian red (2)",
        "Red-yellow",
        "Green",
        "Green & signal"]


    function getX(index) {
        return [0.5353901996, 0.6851179673, 0.7304900181, 0.8484573503, 0.7304900181, 0.6851179673, 0.5353901996, 0.5308529946, 0.4355716878][index]
    }

    function getY(index) {
        return [0.2447552448, 0.2447552448, 0.3671328671, 0.3671328671, 0.5769230769, 0.7226107226, 0.7226107226, 0.5769230769, 0.3671328671][index]
    }

    function showState(state) {
        display_img.source = 'qrc:/fsm/Images/' + lut.indexOf(state) + '.png'
    }

    function getTiming(index) {
        return ["A", "B", "C", "C'", "D", "E", "F", "G", "H"][index];
    }

    Timer {
        interval: 500
        repeat: true
        running: true
        onTriggered: {
            RESTClient.get_signal((state)=>pedsignal.visible = state == 1, console.log);
            RESTClient.get_stopped((state)=>carsignal.visible = state == 1, console.log);
            RESTClient.get_state((state)=>showState(state), console.log);
        }
    }

    Repeater{
        id: spinboxes
        model: 9

        SpinBox {
               from: 10
               value: 1000
               to: 10000
               stepSize: 100
               width: 60
               height: 30
               editable: true
               x: display_img.width*getX(index)
               y: display_img.height*getY(index)
               onValueChanged: RESTClient.set_time(display_img.getTiming(index), value, ()=>{}, console.log);
        }

        Component.onCompleted: {
            RESTClient.get_time("A", (time)=> spinboxes.itemAt(0).value = +(time), console.log);
            RESTClient.get_time("B", (time)=> spinboxes.itemAt(1).value = +(time), console.log);
            RESTClient.get_time("C", (time)=> spinboxes.itemAt(2).value = +(time), console.log);
            RESTClient.get_time("C'", (time)=> spinboxes.itemAt(3).value = +(time), console.log);
            RESTClient.get_time("D", (time)=> spinboxes.itemAt(4).value = +(time), console.log);
            RESTClient.get_time("E", (time)=> spinboxes.itemAt(5).value = +(time), console.log);
            RESTClient.get_time("F", (time)=> spinboxes.itemAt(6).value = +(time), console.log);
            RESTClient.get_time("G", (time)=> spinboxes.itemAt(7).value = +(time), console.log);
            RESTClient.get_time("H", (time)=> spinboxes.itemAt(8).value = +(time), console.log);
        }
    }

    Column{
        anchors.margins: 10
        Text{
            id: pedsignal
            visible: false
            text: "Pedestrian signal pending"
        }
        Text{
            id: carsignal
            visible: false
            text: "Car stopped signal pending"
        }
    }

}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
