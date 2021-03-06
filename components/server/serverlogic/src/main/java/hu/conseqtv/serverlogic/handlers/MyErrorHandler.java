package hu.conseqtv.serverlogic.handlers;

import hu.conseqtv.ErrorHandler;
import hu.conseqtv.serverlogic.storage.log.Log;
import hu.conseqtv.serverlogic.storage.log.LogRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

@Component
public class MyErrorHandler implements ErrorHandler {

    LogRepository logRepository;

    @Autowired
    public MyErrorHandler(LogRepository logRepository) {
        this.logRepository = logRepository;
    }

    @Override
    public void handle(String s) {
        logRepository.save(new Log(s, 2));
    }
}
