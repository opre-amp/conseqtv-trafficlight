import QtQuick 2.14
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.StateMachine 1.14 as DSM

import userdata 1.0
import delaysettings 1.0

import 'RESTclient.js' as RESTClient


Flickable{

    anchors.fill: parent
    anchors.bottomMargin: 20
    anchors.margins: 10


    RowLayout{
        id: main_layout
        anchors.margins: 20

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

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: set_state
            text: "Set State"
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }

        Rectangle{
            width: cols.width
            height: parent.height
        }
        Button{
            id: send_police
            text: "Send Police Interrupt"
            enabled: RESTClient.is_police()

            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: send_switch_on
            text: "Switch On"
            enabled: RESTClient.is_police() && RESTClient.is_off()
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }
        Button{
            id: send_switch_off
            text: "Switch Off"
            enabled: RESTClient.is_police() && !RESTClient.is_off()
            Layout.alignment: Qt.AlignLeft | Qt.AlignTop
        }


    }
    Loader{
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
