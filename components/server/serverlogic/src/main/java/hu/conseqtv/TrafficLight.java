package hu.conseqtv;

import java.util.Arrays;
import java.util.List;

public class TrafficLight {

    private static final String[] states = {
            "Off",
            "Blinking yellow",
            "Yellow",
            "Red & pedestrian red (1)",
            "Red & pedestrian green",
            "Red & pedestrian green & stop",
            "Red & blinking pedestrian green",
            "Red & pedestrian red (2)",
            "Red-yellow",
            "Green",
            "Green & signal"
    };
    private static final String[] signals = {
            "switch1",
            "switch0",
            "police!"
    };

    public TrafficLight() {
        initMailbox();
    }

    public static List<String> getStates() {
        return Arrays.asList(states);
    }

    public static List<String> getSignals() {
        return Arrays.asList(signals);
    }

    public void close() {
        uninitMailbox();
    }

    private int initMailbox(){return 1;}
    private int uninitMailbox(){return 1;}

    public void registerHeartbeatHandler(HeartbeatHandler handler){}
    public void registerErrorHandler(ErrorHandler handler){}

    public String getState(){return states[1];}

    public int setState(String state){return 1;}

    public int sendSignal(String signal){return 1;}

    public int setTimeA(int time){return 1;}
    public int setTimeB(int time){return 1;}
    public int setTimeC(int time){return 1;}
    public int setTimeC_(int time){return 1;}
    public int setTimeD(int time){return 1;}
    public int setTimeE(int time){return 1;}
    public int setTimeF(int time){return 1;}
    public int setTimeG(int time){return 1;}
    public int setTimeH(int time){return 1;}

    public int getTimeA(){return 1;}
    public int getTimeB(){return 1;}
    public int getTimeC(){return 1;}
    public int getTimeC_(){return 1;}
    public int getTimeD(){return 1;}
    public int getTimeE(){return 1;}
    public int getTimeF(){return 1;}
    public int getTimeG(){return 1;}
    public int getTimeH(){return 1;}

    public int testRed(){return 1;}
    public int testYlw(){return 1;}
    public int testGrn(){return 1;}
    public int testPred(){return 1;}
    public int testPgrn(){return 1;}

    public int getStopped(){return 0;}
    public int getSignal(){return 0;}

}
