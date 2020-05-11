import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import tablemodel 1.0

import 'RESTclient.js' as RESTClient

Flickable {
    id: fparent
    anchors.fill: parent
    anchors.bottomMargin: 20
    anchors.margins: 10


    interactive: true
    clip: true
    flickableDirection: Flickable.VerticalFlick
    contentHeight: messagesList.height + user_mgmt.height + r_layout.height

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
            }

            Button {
                id: modifyuser
                text: qsTr("Modify User")
            }

            Button {
                id: deleteuser
                text: qsTr("Delete User")
            }



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
            height: messageField.height
            color: "lavender"

            TextField {
                id: messageField
                width: base.width
                Layout.fillHeight: true
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
                messagesList.model.sendMessage("Me",group.currentText, messageField.text);
                messageField.text = "";
            }

            onClicked: send()
        }
    }


    ListView {
        id: messagesList
        anchors.top: r_layout.bottom
        width: parent.width
        height: childrenRect.height
        clip: true
        model: MessageTableModel {
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
                    height: messageText.implicitHeight
                    Text{
                        id: messageText
                        width: fparent.width
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
        onCountChanged: {
            messagesList.positionViewAtEnd()
        }
    }
}



/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
