import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import tablemodel 1.0

Flickable {
        id: fparent
        anchors.fill: parent
        anchors.bottomMargin: 20
        anchors.margins: 10


        interactive: true
        clip: true
        flickableDirection: Flickable.VerticalFlick
        contentHeight: messagesList.height *1.5


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
                   width: messageText.implicitWidth + 20
                   height: messageText.implicitHeight + 20
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
        RowLayout {
            id: r_layout
            width: parent.width
            Rectangle{
                id: base
                Layout.fillWidth: true
                height: messageField.height
                color: "lavender"

            TextArea {
                id: messageField
                width: base.width
                Layout.fillHeight: true
                focus: true
                font.family: "Courier New"
                font.pointSize: 12
                wrapMode: TextArea.Wrap
                placeholderText: qsTr("Compose message")
            }
            }
            ComboBox {
                id: group
                model: [ "All", "Maintenance", "Ploice", "Research" ]
            }
            Button {
                id: sendButton
                font.family: "Courier New"

                text: qsTr("Send")
                enabled: messageField.length > 0
                onClicked: {
                    messagesList.model.sendMessage("Me",group.currentText, messageField.text);
                    messageField.text = "";
                }
            }
         }
}


