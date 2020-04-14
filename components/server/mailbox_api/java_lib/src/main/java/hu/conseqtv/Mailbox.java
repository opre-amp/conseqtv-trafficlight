package hu.conseqtv;

public class Mailbox {

    public static void main(String[] args) {
        Mailbox mailbox = new Mailbox();
        mailbox.register_error_handler(new ErrorHandler(){
            public void handle(String s) { System.out.println(s);}
        });
        mailbox.register_heartbeat_handler(new HeartbeatHandler(){
            public void handle() { System.out.println("Heartbeat");}
        });
        mailbox.send_signal("switch1");
        try{
            Thread.sleep(3000);
        }catch(Exception e){e.printStackTrace();}
        mailbox.send_signal("police!");
        mailbox.send_signal("police_");
    }

    public Mailbox() {
        init_mailbox();
    }

    @Override
    protected void finalize() throws Throwable {
        uninit_mailbox();
        super.finalize();
    }

    static {
        System.loadLibrary("pthread");
        System.loadLibrary("mailbox");
        System.loadLibrary("mailbox_javacompat");
    }

    private native int init_mailbox();
    private native int uninit_mailbox();

    public native void register_heartbeat_handler(HeartbeatHandler handler);
    public native void register_error_handler(ErrorHandler handler);

    public native String get_state();

    public native int set_state(String state);

    public native int send_signal(String signal);

    public native int set_time_A(int time);
    public native int set_time_B(int time);
    public native int set_time_C(int time);
    public native int set_time_C_(int time);
    public native int set_time_D(int time);
    public native int set_time_E(int time);
    public native int set_time_F(int time);
    public native int set_time_G(int time);
    public native int set_time_H(int time);

    public native int test_red();
    public native int test_ylw();
    public native int test_grn();
    public native int test_pred();
    public native int test_pgrn();

}
