package hu.conseqtv.serverlogic.payload.request;
import javax.validation.constraints.NotBlank;
import javax.validation.constraints.Size;

public class ModificationRequest {
    @NotBlank
    @Size(min = 3, max = 20)
    private String username;

    private String role;

    @Size(min = 6, max = 40)
    private String password;

    public String getUsername() {
        return username;
    }

    public void setUsername(String username) {
        this.username = username;
    }

    public String getPassword() {
        return password;
    }

    public void setPassword(String password) {
        this.password = password;
    }

    public String getRole() {
        return this.role;
    }

    public void setRole(String role) {
        this.role = role;
    }
}