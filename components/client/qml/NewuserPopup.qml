import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import tablemodel 1.0

import 'RESTclient.js' as RESTClient

Dialog {
    title: "Adding new user"
    modal: true
    x: (parent.width - width) / 2
    y: (parent.height - height) / 2
    standardButtons: Dialog.Ok | Dialog.Cancel
    contentItem: Rectangle {
        implicitWidth: Window.width/3
        implicitHeight: Window.height/3
        anchors.fill: parent

        Text{
            id: errorMsg
            color: 'red'
            text: "error msg"
            horizontalAlignment: Text.AlignHCenter
            visible: false;
            width: grid.width
            anchors.bottom: grid.top
            anchors.horizontalCenter: grid.horizontalCenter
        }
        
        Grid {
            id: grid
            rows: 3
            columns: 2
            anchors.centerIn: parent
            
            Text{
                text: "Username: "
            }
            TextField{
                id: username
                width: 300
                placeholderText: "bob"
                autoScroll: true
                font.family: "Courier New"
                font.pointSize: 12
            }
            
            Text{
                text: "Password: "
            }
            TextField{
                id: passw
                echoMode: "Password"
                passwordCharacter: '*'
                placeholderText: "*******"
                width: 300
                autoScroll: true
                font.family: "Courier New"
                font.pointSize: 12
            }
            
            
            Text{
                text: "Role: "
            }
            ComboBox{
                id: newuser_role
                flat: true
                model: ["user", "police", "admin"]
                
            }
            
            
        }

    }
    onAccepted: RESTClient.new_user(username.text, passw.text, newuser_role.currentValue, console.log, (response)=>{visible = true; errorMsg.text = response; errorMsg.visible = true; console.log(respone)});
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
