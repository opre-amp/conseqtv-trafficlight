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
    contentHeight: messagesList.height + user_mgmt.height*2 + r_layout.height*2

    RowLayout {
        id: user_mgmt
        anchors.margins: 20
        enabled: RESTClient.is_admin()

            Text {
                id: element
                text: qsTr("User management: ")
                font.pixelSize: 12
            }

            Button {
                id: newuser
                text: qsTr("Add User")
                onClicked: newuser_popup.open()
            }

            Button {
                id: modifyuser
                text: qsTr("Modify User")
                onClicked: moduser_popup.open()
            }

            Button {
                id: deleteuser
                text: qsTr("Delete User")
                onClicked: deluser_popup.open()
            }
    }
    NewuserPopup {
        id: newuser_popup
    }
    ModuserPopup {
        id: moduser_popup
    }
    DeluserPopup {
        id: deluser_popup
    }

    RowLayout {
        id: r_layout
        x: 0
        width: parent.width
        anchors.topMargin: 8
        anchors.top: user_mgmt.bottom
        Rectangle{
            id: base
            Layout.fillWidth: true
            height: messageField.implicitHeight
            color: "lavender"

            TextField {
                id: messageField
                width: base.width
                focus: true
                font.family: "Courier New"
                font.pointSize: 12
                wrapMode: TextArea.Wrap
                placeholderText: qsTr("Compose message")
                Keys.onEnterPressed: sendButton.send()
                Keys.onReturnPressed: sendButton.send()
                Component.onCompleted: messageField.forceActiveFocus()
            }
        }
        ComboBox {
            id: group
            model: [ "User", "Police", "Admin" ]
        }
        Button {
            id: sendButton
            font.family: "Courier New"

            text: qsTr("Send")
            enabled: messageField.length > 0
            function send() {
                if(messageField.length > 0) RESTClient.post_message(RESTClient.get_username(), messageField.text, group.currentValue, ()=>messageField.text = "", console.log)
            }
            onClicked: send()
        }
        property var last_msg: 0
        Component.onCompleted: RESTClient.get_messages_all(
                                   (text)=>{
                                       JSON.parse(text).forEach((msg)=>{
                                                message_list_model.insert(0, msg);
                                                r_layout.last_msg = +(msg.timestamp);
                                            });
                                            message_sync.running = true;
                                }, console.log)

        Timer {
            id: message_sync
            interval: 500
            repeat: true
            running: false
            onTriggered: {
                RESTClient.get_messages_filtered(r_layout.last_msg,
                                                 (text)=>{
                                                     JSON.parse(text).forEach((msg)=>{
                                                              message_list_model.insert(0, msg);
                                                              r_layout.last_msg = +(msg.timestamp);
                                                          });
                                              }, console.log)
            }

            Component.onDestruction: message_sync.running = false
        }
    }


    ListView {
        id: messagesList
        anchors.top: r_layout.bottom
        width: parent.width
        height: childrenRect.height
        clip: true
        model: ListModel {
            id: message_list_model;
        }
        delegate:
            Column{
            Row {
                RowLayout{
                    id: messageRow
                    Label {
                        id: timestampText
                        text: Qt.formatDateTime(model.timestamp, "d MMM hh:mm")
                        font.family: "Courier New"
                        font.pointSize: 12
                        font.italic: true
                        color: "black"
                    }
                    Label {
                        id: authorText
                        text: model.author
                        font.family: "Courier New"
                        font.pointSize: 12
                        color: "black"
                    }

                    Label {
                        id: rarrowText
                        text: "→"
                        font.family: "Courier New"
                        font.pointSize: 12
                        color: "black"
                    }
                    Label {
                        id: groupText
                        text: model.group
                        font.family: "Courier New"
                        font.pointSize: 12
                        color: "black"
                    }
                }
            }
            Row{
                Rectangle {
                    width: fparent.width
                    height: messageText.height + 10
                    Text{
                        id: messageText
                        width: fparent.width - 10
                        anchors.centerIn: parent
                        text: model.message
                        font.family: "Courier New"
                        font.pointSize: 12
                        clip: true
                        wrapMode: Text.Wrap
                    }
                }
            }

        }
        interactive: true
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
