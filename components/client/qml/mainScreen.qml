import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 1.4
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3
import QtQuick.Controls.Styles 1.4

import 'RESTclient.js' as RESTClient

Item {
    id: mainScreenItem
    width: Window.width
    height: Window.height

    signal signalScreenChange(string screenPath)

    Row{
        id: header
        width: Window.width/2
        height: 75
        anchors.left: col_1.right
        anchors.right: col_2.left

        Rectangle{
            id: headline
            width: header.width
            height: header.height
            color: "black"

        Image {
            id: logo
            anchors.verticalCenter: headline.verticalCenter
            source:'qrc:/background/Images/icon.png'
            height: 60
            width: 60
        }

        Label {
            id: title
            anchors.left: logo.right
            anchors.verticalCenter: headline.verticalCenter
            font.family: "Courier New"
            font.pointSize: 26
            text: "CONSEQTV"
            color: "white"
        }
        Timer {
            id: reachable_timer
            interval: 500
            running: true
            repeat: true
            onTriggered: {
                RESTClient.get_heartbeat();
                server_status.text = RESTClient.is_reachable() ? "Available" : "Unavailable";
                device_status.text = RESTClient.is_device_reachable() ? "Available" : "Unavailable";
            }

            Component.onDestruction: reachable_timer.running = false
        }

        Row{
            id: row_1
            anchors.right: headline.right
            Label{
                text: "Server status: "
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
            }
            Text{
                id: server_status
                text: "Unavailable"
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
            }
        }
        Row{
            id: row_2
            anchors.right: headline.right
            anchors.top: row_1.bottom
            Label{
                text: "Device status: "
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
            }
            Text {
                id: device_status
                text: "Unavailable"
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
            }
        }

        Row{
            id: row_3
            anchors.right: headline.right
            anchors.top: row_2.bottom
            anchors.bottom: headline.bottom
            Timer {
                    id: text_timer
                    interval: 1000
                    repeat: true
                    running: true
                    triggeredOnStart: true
                    onTriggered: time_text.text = Qt.formatTime(new Date(), "hh:mm:ss")
                    Component.onDestruction: text_timer.running = false
            }
            Text {
                id: time_text
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
                text: "00:00:00"
            }
        }
        }
    }


    Column {
        id: col_1
        anchors.left: mainScreenItem.left
        Rectangle {
            width: mainScreenItem.width/10
            height: mainScreenItem.height
            color: "black"
        }
    }
    TabView {
        id: tabview
        anchors.left: col_1.right
        anchors.top: header.bottom
        height: mainScreenItem.height
        width: (mainScreenItem.width/5)*4

        Tab {
            id: hometab
            title: "Home"
            source: 'qrc:/homeScreen.qml'
        }
        Tab {
            id: controltab
            title: "Controls"
            source: 'qrc:/controlScreen.qml'
        }
        Tab {
            id: stattab
            title: "Logs"
            source: 'qrc:/statScreen.qml'
            enabled: RESTClient.is_admin()
        }

        Tab{
            id: log
            title: "Log out"
            enabled: false

        }

        style: TabViewStyle {
            frameOverlap: 1
            tab: Rectangle {
                color: styleData.selected ? "lightgray" : "darkgray"
                border.color: styleData.selected ? "lightgray" : "darkgray"
                implicitWidth: tabview.width/4
                implicitHeight: 50
                radius: 0
                Text {
                    id: textid
                    anchors.centerIn: parent
                    text: styleData.title !== "Log out" ? styleData.title : ""
                    color: styleData.selected ? "black" : "white"
                    font.family: "Arial"
                    font.bold: true
                }
                Button{
                    visible: styleData.title !== "Log out" ? false : true
                    anchors.left: parent.left
                    anchors.right: parent.right
                    anchors.verticalCenter: parent.verticalCenter
                    text: "Log out"
                    onClicked: logout()

                    function logout(){
                        RESTClient.sign_out()
                        mainScreenItem.signalScreenChange("loginScreen.qml")
                    }
                }
            }

            frame: Rectangle { color: "lightgray" }

         }
    }

    Column {
        id: col_2
        anchors.left: tabview.right
        anchors.right: mainScreenItem.right
        Rectangle {
            width: mainScreenItem.width/10
            height: mainScreenItem.height
            color: "black"
        }
    }
}


