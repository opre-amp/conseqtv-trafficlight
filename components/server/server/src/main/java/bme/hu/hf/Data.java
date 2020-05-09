package bme.hu.hf;

import javax.persistence.Entity;
import javax.persistence.Id;
import java.time.LocalDateTime;

@Entity
public class Data {
    @Id
    private Long key;
    private String value;
    private LocalDateTime timestamp;

    public Long getKey() {
        return key;
    }
    public void setKey(Long key) {
        this.key = key;
    }

    public String getValue() {
        return value;
    }
    public void setValue(String value) {
        this.value = value;
    }

    public LocalDateTime getTimestamp() { return timestamp; }
    public void setTimestamp(LocalDateTime timestamp) { this.timestamp = timestamp; }
}
