import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4


import 'RESTclient.js' as RESTClient

Flickable {
    id: fparent
    anchors.fill: parent
    anchors.bottomMargin: 20
    anchors.margins: 10


    interactive: true
    clip: true
    flickableDirection: Flickable.VerticalFlick
    contentHeight: logList.height + 60 + r_layout.height


    RowLayout {
        id: r_layout
        x: 0
        anchors.right: parent.right
        Button {
            text: "Clear"
            onClicked: {
                log_list_model.clear();
                logList.first_log = logList.last_log;
            }
        }

        ComboBox{
            model: ["info", "warning", "error"]
            id: loglevel_combobox
            onCurrentValueChanged: {
                log_sync.running = false;
                logList.last_log = logList.first_log;
                log_list_model.clear();
                log_sync.running = true;
            }
        }
    }


    ListView {
        anchors.top: r_layout.bottom
        id: logList
        width: parent.width
        height: childrenRect.height
        clip: true
        function get_color(loglevel) {
            switch(loglevel) {
            case 2: return "red";
            case 1: return "orange";
            default: return "black";
            }
        }
        function get_name(loglevel) {
            switch(loglevel) {
            case 2: return "error";
            case 1: return "warning";
            default: return "info";
            }
        }
        model: ListModel {
            id: log_list_model;
        }
        delegate:
            Column{
            Row {
                RowLayout{
                    id: logRow
                    Label {
                        id: timestampText
                        text: Qt.formatDateTime(new Date(model.creationDateTime), "d MMM hh:mm:ss")
                        font.family: "Courier New"
                        font.pointSize: 12
                        font.italic: true
                        color: "black"
                    }
                    Label {
                        id: levelText
                        text: logList.get_name(model.logLevel)
                        font.family: "Courier New"
                        font.pointSize: 12
                        color: logList.get_color(model.logLevel)
                    }
                }
            }
            Row{
                Rectangle {
                    width: fparent.width
                    height: messageText.height+10
                    Text{
                        anchors.centerIn: parent
                        id: messageText
                        width: fparent.width - 10
                        text: model.message
                        font.family: "Courier New"
                        font.pointSize: 12
                        clip: true
                        wrapMode: Text.Wrap
                        color: logList.get_color(model.logLevel)
                    }
                }
            }

        }
        property var first_log: (new Date()).valueOf()
        property var last_log: first_log

        Timer {
            id: log_sync
            interval: 1000
            repeat: true
            running: true
            onTriggered: {
                RESTClient.get_filtered_logs(logList.last_log,loglevel_combobox.currentIndex,
                                                 (text)=>{
                                                     JSON.parse(text).forEach((log)=>{
                                                                if(+(log.creationDateTime) > logList.first_log && log.logLevel >= loglevel_combobox.currentIndex) {
                                                                log_list_model.insert(0, log);
                                                                logList.last_log = +(log.creationDateTime);
                                                                }
                                                          });
                                              }, console.log)
            }
            Component.onDestruction: log_sync.running = false
        }
        interactive: true
        onModelChanged: {
            messagesList.positionViewAtEnd()
        }
    }
}
