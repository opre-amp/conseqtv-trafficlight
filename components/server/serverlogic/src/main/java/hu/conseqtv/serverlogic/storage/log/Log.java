package hu.conseqtv.serverlogic.storage.log;

import javax.persistence.*;

@Entity
@Table(name = "log")
public class Log {
    @Id
    @GeneratedValue(strategy = GenerationType.IDENTITY)
    private Integer id;

    private long creationDateTime;

    @Column(length = 255)
    private String message;

    private int logLevel;

    public Log() {}
    public Log(String message,int level) {
        this.message = message;
        this.logLevel = level;
        this.creationDateTime = System.currentTimeMillis();
    }

    public Integer getId() {
        return id;
    }

    public void setId(Integer id) {
        this.id = id;
    }

    public long getCreationDateTime() {
        return creationDateTime;
    }

    public void setCreationDateTime(long creationDateTime) {
        this.creationDateTime = creationDateTime;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public int getLogLevel() {
        return logLevel;
    }

    public void setLogLevel(int logLevel) {
        this.logLevel = logLevel;
    }
}