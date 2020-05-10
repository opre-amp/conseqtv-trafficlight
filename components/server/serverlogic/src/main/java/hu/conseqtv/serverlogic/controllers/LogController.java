package hu.conseqtv.serverlogic.controllers;

import hu.conseqtv.serverlogic.payload.request.LogRequest;
import hu.conseqtv.serverlogic.storage.log.Log;
import hu.conseqtv.serverlogic.storage.log.LogRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.GetMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import javax.validation.Valid;
import java.util.List;

@RestController
@RequestMapping("/logs")
public class LogController {

    @Autowired
    private LogRepository logRepository;


    public LogController() {
    }

    @GetMapping("/filtered")
    @PreAuthorize("hasRole('ADMIN')")
    public List<Log> retrieveFiltered(@Valid @RequestBody LogRequest logRequest) {
        return logRepository.findAllWithCreationDateTimeAfter(logRequest.getSince(), logRequest.getMinLogLevel());
    }

    @GetMapping("/all")
    @PreAuthorize("hasRole('ADMIN')")
    public List<Log> retrieveAll() {
        return logRepository.findAll();
    }

}
