package hu.conseqtv.serverlogic.payload.request;

import javax.validation.constraints.NotBlank;
import java.util.Set;

public class TestRequest {
    @NotBlank
    private Set<String> tests;

    public Set<String> getTests() {
        return tests;
    }

    public void setTests(Set<String> tests) {
        this.tests = tests;
    }
}