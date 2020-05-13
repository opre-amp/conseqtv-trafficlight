import QtQuick 2.12
import QtQuick.Layouts 1.12
import QtQuick.Window 2.10
import QtQuick.Controls 2.12
import QtQuick.Controls.Styles 1.4

import 'RESTclient.js' as RESTClient

Dialog {
    title: "Modifying user"
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
            columns: 3
            anchors.centerIn: parent

            Item{
                width: 1
                height: 1
            }

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
            CheckBox{
                id: update_passw
                checked: true
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
                enabled: update_passw.checked
            }
            CheckBox{
                id: update_role
                checked: true
            }

            Text{
                text: "Role: "
            }
            ComboBox{
                enabled: update_role.checked
                id: newuser_role
                flat: true
                model: ["user", "police", "admin"]
                
            }
            
            
        }
    }

    onAccepted: {
        let user = new Object();
        user.username = username.text;
        if(update_passw.checked) user.password = passw.text;
        if(update_role.checked) user.role = newuser_role.currentValue;
        RESTClient.modify_user(user, console.log, (response)=>{visible = true; errorMsg.text = response; errorMsg.visible = true; console.log(response)});
    }
}

/*##^##
Designer {
    D{i:0;autoSize:true;height:480;width:640}
}
##^##*/
