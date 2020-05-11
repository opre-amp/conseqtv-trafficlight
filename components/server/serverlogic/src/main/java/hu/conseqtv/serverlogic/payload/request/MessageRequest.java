package hu.conseqtv.serverlogic.payload.request;

import hu.conseqtv.serverlogic.storage.roles.ERole;

import javax.validation.constraints.NotBlank;
import javax.validation.constraints.Size;

public class MessageRequest {
    @NotBlank
    @Size(min = 3, max = 20)
    private String username;

    @NotBlank
    private String role;

    @NotBlank
    private String message;

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getRole() {
        return role;
    }

    public void setRole(String role) {
        this.role = role;
    }

    public String getMessage() {
        return message;
    }

    public void setMessage(String message) {
        this.message = message;
    }


    public ERole getERole() {
        switch(role.toLowerCase()) {
            case "admin": return ERole.ROLE_ADMIN;
            case "police": return ERole.ROLE_POLICE;
            default: return ERole.ROLE_USER;
        }
    }
}
