import QtQuick 2.10
import QtQuick.Window 2.10
import QtQuick.Controls 2.3
import QtQuick.Layouts 1.3



Image {
    id: backgroundimg
    source: "qrc:/background/Images/NewYork_Lights2.png"
    anchors.fill: parent
    signal signalScreenChange(string screenPath)


    ColumnLayout{

            anchors.horizontalCenter: parent.horizontalCenter
            anchors.verticalCenter: parent.verticalCenter


            Label {
                    horizontalAlignment: "AlignHCenter"
                    width: parent.width
                    font.family: "Courier New"
                    font.pointSize: 36
                    text: "CONSEQTV"
                    color: "white"
                }
            Label{
                width: 10
                height: 80
            }

            Label {
                horizontalAlignment: "AlignLeft"
                font.family: "Courier New"
                font.pointSize: 12
                text: "Username"
                color: "white"
            }

            Rectangle{
               id: usrbg
               color: "white"
               Layout.preferredWidth: Math.max(backgroundimg.width / 6, 100)
               height: 25

               TextInput{
                   id: usrname
                   visible: true
                   width: usrbg.width
                   autoScroll: true
                   focus: true
                   font.family: "Courier New"
                   font.pointSize: 12
                   clip: true
                   KeyNavigation.tab: passw
               }
            }

            Label {
                text: "Password"
                horizontalAlignment: "AlignLeft"
                font.family: "Courier New"
                font.pointSize: 12
                color: "white"
            }

            Rectangle{
               id: passbg
               color: "white"
               Layout.preferredWidth: Math.max(backgroundimg.width / 6, 100)
               height: 25

               TextInput{
                   id: passw
                   echoMode: "Password"
                   visible: true
                   width: passbg.width
                   autoScroll: true
                   focus: true
                   font.family: "Courier New"
                   font.pointSize: 12
                   clip: true
               }
            }
            Label{
                width: 10
                height: 90
            }

            Button{

                function getAccess() {
                    var xmlhttp = new XMLHttpRequest();
                    var url = "OUR REST ADDRESS";
                    var username = usrname.text;
                    var password = passw.text;
                    var md5 = Qt.md5(password);

                    if (username == ''){
                        return console.log('Missing username');
                    }
                    if (password == ''){
                        return console.log('Missing password');
                    }

                   backgroundimg.signalScreenChange("mainScreen.qml")

                   console.log(username+' '+md5);
                }

                id: loginbtn
                Layout.preferredWidth: Math.max(main_rectangle.width / 6, 100)
                font.family: "Courier New"
                font.pointSize: 12
                text: "Log in"
                onClicked: getAccess()

            }

            Button{
                function exit() {
                    Qt.quit()
                }
                id: exitbtn
                Layout.preferredWidth: Math.max(main_rectangle.width / 6, 100)
                font.family: "Courier New"
                font.pointSize: 12
                text: "Exit"
                onClicked: exit()
            }

        }
}
