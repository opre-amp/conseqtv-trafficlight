package hu.conseqtv.serverlogic.payload.response;

import hu.conseqtv.serverlogic.storage.messageboard.Message;

public class MessageItemResponse {

    private String author;

    private String message;

    private Long timestamp;

    private String group;

    public MessageItemResponse(Message message) {
        this.author = message.getUsername();
        this.message = message.getMessage();
        this.timestamp = message.getTimestamp();
        switch(message.getRole()) {
            case ROLE_USER: this.group = "user"; break;
            case ROLE_POLICE: this.group = "police"; break;
            case ROLE_ADMIN: this.group = "admin"; break;
        }
    }

    public String getAuthor() {
        return author;
    }

    public void setAuthor(String author) {
        this.author = author;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }

    public Long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(Long timestamp) {
        this.timestamp = timestamp;
    }

    public String getGroup() {
        return group;
    }

    public void setGroup(String group) {
        this.group = group;
    }

}
