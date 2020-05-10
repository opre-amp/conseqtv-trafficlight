import QtQuick 2.14
import QtQuick.Window 2.10
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.3
import QtQml.StateMachine 1.14 as DSM

import delaysettings 1.0


Image{
    id: display_img
    source: 'qrc:/background/Images/Working.PNG'

    DelaySettings{id:getter}

DSM.StateMachine {
    id: stateMachine
    initialState: working
    running: true

        DSM.State {
            id: working
            initialState: yellow_p_red



            DSM.State {
                id: yellow_p_red
                onEntered: display_img.source = 'qrc:/background/Images/state1.png'

                DSM.TimeoutTransition {
                    id: time_A
                    targetState: red_p_red
                }
            }
            DSM.State {
                id: red_p_red
                onEntered: display_img.source = 'qrc:/background/Images/state2.png'
                DSM.TimeoutTransition {
                    id: time_B
                    targetState: red_p_green
                }
            }
            DSM.State {
                id: red_p_green
                onEntered: display_img.source = 'qrc:/background/Images/state3.png'
                DSM.SignalTransition {
                    targetState: red_p_green_stopped
                }
                DSM.TimeoutTransition {
                    id: time_C
                    targetState: red_p_blink_green
                }
            }
            DSM.State {
                id: red_p_green_stopped
                onEntered: display_img.source = 'qrc:/background/Images/stopped.png'
                DSM.TimeoutTransition {
                    id: time_C_delta
                    targetState: red_p_blink_green
                }
            }
            DSM.State {
                id: red_p_blink_green
                onEntered: display_img.source = 'qrc:/background/Images/state4.png'
                DSM.TimeoutTransition {
                    id: time_D
                    targetState: red_p_red_after_blink
                }
            }
            DSM.State {
                id: red_p_red_after_blink
                onEntered: display_img.source = 'qrc:/background/Images/state5.png'
                DSM.TimeoutTransition {
                    id: time_E
                    targetState: red_yellow_p_red
                }
            }
            DSM.State {
                id: red_yellow_p_red
                onEntered: display_img.source = 'qrc:/background/Images/state6.png'

                DSM.TimeoutTransition {
                    id: time_F
                    targetState: green_p_red
                }
            }
            DSM.State {
                id: green_p_red
                onEntered: display_img.source = 'qrc:/background/Images/state7.png'
                DSM.TimeoutTransition {
                    id: time_G
                    targetState: green_p_red_before_yellow
                }
                DSM.SignalTransition {
                    targetState: green_p_red_before_yellow
                }
            }
            DSM.State {
                id: green_p_red_before_yellow
                onEntered: display_img.source = 'qrc:/background/Images/state8.png'
                DSM.TimeoutTransition {
                    id: time_H
                    targetState: yellow_p_red
                }
            }
        }
    }
}


