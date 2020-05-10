import QtQuick 2.14
import QtQuick.Window 2.10
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import QtQml.StateMachine 1.14 as DSM

import userdata 1.0
import delaysettings 1.0


Flickable{

DelaySettings{
    id: setter
}

RowLayout{
    id: main_layout
//    anchors.horizontalCenter: parent.horizontalCenter


    Column{
        id: pack_1
        padding:10
        Repeater{
        id: repeat_1
        model:5
        Row{
            Label{
                text: ("time_"+(model.index+1)).toString()
                font.family: "Courier New"
                font.pointSize: 12
                padding: 10
                anchors.verticalCenter: parent.verticalCenter

            }

            SpinBox {
                   id: index_1
                   from: 0
                   value: 100
                   to: 100 * 100
                   stepSize: 10


                   property int decimals: 2
                   property real realValue: value / 100
                   property string label: ("time_"+(model.index+1)).toString()

                   validator: DoubleValidator {
                       bottom: Math.min(index_1.from, index_1.to)
                       top:  Math.max(index_1.from, index_1.to)
                   }

                   textFromValue: function(value, locale) {
                       console.log(label +' '+ value)
                       setter.setTime(label, value*10)
                       return Number(value / 100).toLocaleString(locale, 'f', index_1.decimals)
                   }

                   valueFromText: function(text, locale) {
                       return Number.fromLocaleString(locale, text) * 100
                   }
               }
            }
        }
    }

    Column{
        id: pack_2
        Repeater{
        id: repeat_2
        model:4
        Row{
            Label{
                text: ("time_"+(model.index+6)).toString()
                font.family: "Courier New"
                font.pointSize: 12
                padding: 10
                anchors.verticalCenter: parent.verticalCenter

            }
            SpinBox {
                   id: index_2
                   from: 0
                   value: 100
                   to: 100 * 100
                   stepSize: 10


                   property int decimals: 2
                   property real realValue: value / 100
                   property string label: ("time_"+(model.index+6)).toString()

                   validator: DoubleValidator {
                       bottom: Math.min(index_2.from, index_2.to)
                       top:  Math.max(index_2.from, index_2.to)
                   }

                   textFromValue: function(value, locale) {
                       console.log(label +' '+ value)
                       setter.setTime(label, value*10)
                       return Number(value / 100).toLocaleString(locale, 'f', index_2.decimals)
                   }

                   valueFromText: function(text, locale) {
                       return Number.fromLocaleString(locale, text) * 100
                   }
               }
            }
        }
    }
    Column{
        Button{
           text:  "Start"
           onClicked: function getter(){


           }
        }
        Button{
            text: "Stop"
        }

    }

}
Row{
    anchors.top: main_layout.bottom
    padding: 40

    Loader{
        source: 'qrc:/stateMachine.qml'
    }
}

}

