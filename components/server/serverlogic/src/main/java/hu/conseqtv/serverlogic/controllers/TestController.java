package hu.conseqtv.serverlogic.controllers;

import hu.conseqtv.serverlogic.storage.log.Log;
import hu.conseqtv.serverlogic.storage.log.LogRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

@RestController
@RequestMapping("/apitest")
public class TestController {

    @Autowired
    LogRepository logRepository;

    @GetMapping("/all")
    public String allAccess() {
        logRepository.save(new Log("Hello_INFO", 0));
        logRepository.save(new Log("Hello_WARN", 1));
        logRepository.save(new Log("Hello_ERR!", 2));
        return "Public Content.";
    }

    @GetMapping("/user")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public String userAccess() {
        return "User Content.";
    }

    @GetMapping("/police")
    @PreAuthorize("hasRole('POLICE') or hasRole('ADMIN')")
    public String moderatorAccess() {
        return "Police Content.";
    }

    @GetMapping("/admin")
    @PreAuthorize("hasRole('ADMIN')")
    public String adminAccess() {
        return "Admin Content.";
    }
}