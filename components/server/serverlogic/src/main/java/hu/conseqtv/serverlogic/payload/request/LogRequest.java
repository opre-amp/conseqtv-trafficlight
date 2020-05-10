package hu.conseqtv.serverlogic.payload.request;

import javax.persistence.EnumType;
import javax.persistence.Enumerated;
import javax.validation.constraints.NotNull;

public class LogRequest {
    @NotNull
    @Enumerated(EnumType.STRING)
    private int minLogLevel;

    @NotNull
    private Long since;

    public long getSince() {
        return since;
    }

    public void setSince(long since) {
        this.since = since;
    }

    public int getMinLogLevel() {
        return minLogLevel;
    }

    public void setMinLogLevel(int minLogLevel) {
        this.minLogLevel = minLogLevel;
    }
}
