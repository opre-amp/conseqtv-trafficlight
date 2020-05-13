import QtQuick 2.14
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3

import 'RESTclient.js' as RESTClient


Flickable{

    anchors.fill: parent
    anchors.bottomMargin: 20
    anchors.topMargin: 25
    anchors.margins: 10


    RowLayout{
        id: main_layout

        Column{
            id: cols
            CheckBox {
                id: test_red
                text: qsTr("Red")
                checked: true
            }

            CheckBox {
                id: test_ylw
                text: qsTr("Yellow")
                checked: true
            }


            CheckBox {
                id: test_grn
                text: qsTr("Green")
                checked: true
            }

            CheckBox {
                id: test_pedred
                text: qsTr("Pedestrian Red")
                checked: true
            }

            CheckBox {
                id: test_pedgrn
                text: qsTr("Pedestrian Green")
                checked: true
            }
        }
        Button{
            id: request_test
            text: "Request Test"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            onClicked: {
                let arr = new Object();
                arr.tests = [];
                if(test_red.checked) arr.tests.push("red");
                if(test_ylw.checked) arr.tests.push("ylw");
                if(test_grn.checked) arr.tests.push("grn");
                if(test_pedred.checked) arr.tests.push("pred");
                if(test_pedgrn.checked) arr.tests.push("pgrn");
                RESTClient.request_test(arr, (id)=>{
                                            testtimer.testid = id;
                                            testdialog.open();
                                        }, console.log);
            }
        }
        Dialog {
            id: testdialog
            anchors.centerIn: parent
            contentItem: Rectangle{
                implicitHeight: 80
                implicitWidth: 150
                anchors.fill: parent
                Text {
                    anchors.margins: 10
                    id: testtext
                    text: "In progress"
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignTop
                }
            }
            standardButtons: Dialog.Ok
            modal: true
            onAboutToShow: {
                testtext.text = "In progress";
                testtimer.running = true;
            }
            onClosed: testtimer.running = false
            Timer{
                id: testtimer
                property var testid: -1
                interval: 500
                running: false
                repeat: true
                onTriggered: {RESTClient.get_test_status(testid, (text)=>testtext.text = text, console.log)}
            }
        }

        Rectangle{
            width: cols.width
            height: parent.height
        }

        ComboBox {
            id: spinBox
            width: 250
            Layout.maximumWidth: 260
            Layout.minimumWidth: 260
            Layout.preferredWidth: 260

            flat: true
            currentIndex: 0
            editable: false
            model: ["Off",
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
            enabled: RESTClient.is_police()
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: set_state
            text: "Set State"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
            onClicked: RESTClient.set_state(spinBox.currentValue, ()=>{}, console.log);
            enabled: RESTClient.is_police()
        }

        Rectangle{
            width: cols.width
            height: parent.height
        }
        Button{
            id: send_police
            text: "Send Police Interrupt"
            enabled: RESTClient.is_police()
            onClicked: RESTClient.send_police(()=>{}, console.log);

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: send_switch_on
            text: "Switch On"
            enabled: RESTClient.is_police() && RESTClient.is_off()
            onClicked: RESTClient.send_switch(1, ()=>{}, console.log);
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: send_switch_off
            text: "Switch Off"
            enabled: RESTClient.is_police() && !RESTClient.is_off()
            onClicked: RESTClient.send_switch(0, ()=>{}, console.log);
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Timer {
            id: buttontimer
            interval: 100
            repeat: true
            running: RESTClient.is_police()
            onTriggered: {
                send_switch_off.enabled = !RESTClient.is_off();
                send_switch_on.enabled  =  RESTClient.is_off();
            }
            Component.onDestruction: buttontimer.running = false
        }


    }
    Loader{
        anchors.topMargin: 50
        anchors.top: main_layout.bottom
        width: parent.width
        height: 0.389292196*width
        source: 'qrc:/stateMachine.qml'
    }


}


/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
