package hu.conseqtv;

import java.util.Arrays;
import java.util.List;

public class TrafficLight {
    static {
        System.loadLibrary("mailbox_javacompat");
        System.loadLibrary("mailbox");
    }

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

    private native int initMailbox();
    private native int uninitMailbox();

    public native void registerHeartbeatHandler(HeartbeatHandler handler);
    public native void registerErrorHandler(ErrorHandler handler);

    public native String getState();

    public native int setState(String state);

    public native int sendSignal(String signal);

    public native int setTimeA(int time);
    public native int setTimeB(int time);
    public native int setTimeC(int time);
    public native int setTimeC_(int time);
    public native int setTimeD(int time);
    public native int setTimeE(int time);
    public native int setTimeF(int time);
    public native int setTimeG(int time);
    public native int setTimeH(int time);

    public native int testRed();
    public native int testYlw();
    public native int testGrn();
    public native int testPred();
    public native int testPgrn();

}
