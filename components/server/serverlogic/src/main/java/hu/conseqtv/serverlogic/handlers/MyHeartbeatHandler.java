package hu.conseqtv.serverlogic.handlers;

import hu.conseqtv.HeartbeatHandler;

public class MyHeartbeatHandler implements HeartbeatHandler {
    private long lastHeartbeat = 0;
    @Override
    public void handle() {
        lastHeartbeat = System.currentTimeMillis();
    }

    public long getLastHeartbeat() {
        return lastHeartbeat;
    }
}
