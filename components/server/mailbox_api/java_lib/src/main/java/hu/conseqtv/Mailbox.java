package hu.conseqtv;

public class Mailbox {

    private native int init_mailbox();
    private native int uninit_mailbox();

    static {
        System.loadLibrary("mailbox");
    }

    public native void register_heartbeat_handler(HeartbeatHandler handler);
    public native void register_error_handler(ErrorHandler handler);

    public native String get_state();

    public native void set_state(String state);

    public native void send_signal(String signal);

    public native void set_time_A(int time);
    public native void set_time_B(int time);
    public native void set_time_C(int time);
    public native void set_time_C_(int time);
    public native void set_time_D(int time);
    public native void set_time_E(int time);
    public native void set_time_F(int time);
    public native void set_time_G(int time);
    public native void set_time_H(int time);

    public native void test_red();
    public native void test_ylw();
    public native void test_grn();
    public native void test_pred();
    public native void test_pgrn();

}
