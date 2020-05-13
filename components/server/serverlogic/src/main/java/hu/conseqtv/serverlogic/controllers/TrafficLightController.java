package hu.conseqtv.serverlogic.controllers;

import hu.conseqtv.TrafficLight;
import hu.conseqtv.serverlogic.handlers.MyErrorHandler;
import hu.conseqtv.serverlogic.handlers.MyHeartbeatHandler;
import hu.conseqtv.serverlogic.payload.request.TestRequest;
import hu.conseqtv.serverlogic.payload.response.MessageResponse;
import hu.conseqtv.serverlogic.storage.log.Log;
import hu.conseqtv.serverlogic.storage.log.LogRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;
import java.util.HashMap;
import java.util.Map;
import java.util.Set;

@RestController
@RequestMapping("/trafficlight")
public class TrafficLightController {
    private Map<Integer, Test> int2tests;
    private TrafficLight trafficLight;
    @Autowired
    private LogRepository logRepository;
    private MyHeartbeatHandler heartbeatHandler;

    @Autowired
    public TrafficLightController(MyErrorHandler myErrorHandler) {
        trafficLight = new TrafficLight();
        trafficLight.registerHeartbeatHandler(heartbeatHandler = new MyHeartbeatHandler());
        trafficLight.registerErrorHandler(myErrorHandler);
        int2tests = new HashMap<>();
    }

    @GetMapping("/lastheartbeat")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public long getLastHeartbeat(){
        return heartbeatHandler.getLastHeartbeat();
    }


    @PostMapping("/test")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public int createTest(@Valid @RequestBody TestRequest testRequest) {
        StringBuilder tests = new StringBuilder("{");
        testRequest.getTests().forEach(s -> tests.append(s).append(", "));
        logRepository.save(new Log("Creating test for " + tests.toString() + "}", 0));
        return new Test(testRequest.getTests()).id;
    }

    @PostMapping("/inputs/police")
    @PreAuthorize("hasRole('POLICE') or hasRole('ADMIN')")
    public ResponseEntity<?> sendPolice() {
        if(trafficLight.sendSignal(TrafficLight.getSignals().get(2)) == 0) {
            logRepository.save(new Log("Police interrupt sent" , 0));
            return ResponseEntity.ok(new MessageResponse("Police interrupt sent!"));
        }
        else {
            logRepository.save(new Log("Police interrupt could not be sent" , 2));
            return ResponseEntity.badRequest().body(new MessageResponse("Police interrupt could not be sent!"));
        }
    }

    @PostMapping("/inputs/switch")
    @PreAuthorize("hasRole('POLICE') or hasRole('ADMIN')")
    public ResponseEntity<?> sendSwitch(@Valid @RequestParam int state) {
        int status;
        switch (state) {
            case 0: status = trafficLight.sendSignal(TrafficLight.getSignals().get(1)); break;
            case 1: status = trafficLight.sendSignal(TrafficLight.getSignals().get(0)); break;
            default:
                logRepository.save(new Log("Switch on = 1 / off = 0." , 2));
                return ResponseEntity.badRequest().body(new MessageResponse("Switch on = 1 / off = 0."));
        }
        if(status == 0) {
            logRepository.save(new Log("Switch " + (state == 0 ? "off" : "on") +" interrupt sent!" , 0));
            return ResponseEntity.ok(new MessageResponse("Switch " + (state == 0 ? "off" : "on") +" interrupt sent!"));
        }
        else {
            logRepository.save(new Log("Switch interrupt could not be sent" , 2));
            return ResponseEntity.badRequest().body(new MessageResponse("Switch interrupt could not be sent!"));
        }
    }

    @GetMapping("/test/{ID}")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public String getTestState(@PathVariable int ID) {
        logRepository.save(new Log("Retrieving test state" , 0));
        return int2tests.get(ID) == null ? "No such test." : int2tests.get(ID).status;
    }

    @GetMapping("/state")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public String getState() {
        logRepository.save(new Log("Retrieving state" , 0));
        return trafficLight.getState();
    }


    @GetMapping("/sensors/signal")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public int getSignal() {
        logRepository.save(new Log("Retrieving signal" , 0));
        return trafficLight.getSignal();
    }


    @GetMapping("/sensors/stopped")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public int getStopped() {
        logRepository.save(new Log("Retrieving stopped" , 0));
        return trafficLight.getStopped();
    }

    @GetMapping("/times/{timing}")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public int getTime(@PathVariable String timing) {
        logRepository.save(new Log("Retrieving timing" , 0));
        switch(timing) {
            case "A": return trafficLight.getTimeA();
            case "B": return trafficLight.getTimeB();
            case "C": return trafficLight.getTimeC();
            case "C'": return trafficLight.getTimeC_();
            case "D": return trafficLight.getTimeD();
            case "E": return trafficLight.getTimeE();
            case "F": return trafficLight.getTimeF();
            case "G": return trafficLight.getTimeG();
            case "H": return trafficLight.getTimeH();
            default: return -1;
        }
    }

    @PutMapping("/state")
    @PreAuthorize("hasRole('POLICE') or hasRole('ADMIN')")
    public ResponseEntity<?> setState(@Valid @RequestBody String state) {
        logRepository.save(new Log("Setting state to " + state , 0));
        if(trafficLight.setState(state) == 0)
            return ResponseEntity.ok(new MessageResponse("State set to " + state));
        else return ResponseEntity.badRequest().body(new MessageResponse("State could not be set!"));
    }

    @PutMapping("/times/{timing}")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public ResponseEntity<?> setTime(@PathVariable String timing, @Valid @RequestParam int value) {
        logRepository.save(new Log("Setting timing " + timing + " to " + value , 0));
        int status;
        switch(timing) {
            case "A": status = trafficLight.setTimeA(value); break;
            case "B": status = trafficLight.setTimeB(value); break;
            case "C": status = trafficLight.setTimeC(value); break;
            case "C'": status = trafficLight.setTimeC_(value); break;
            case "D": status = trafficLight.setTimeD(value); break;
            case "E": status = trafficLight.setTimeE(value); break;
            case "F": status = trafficLight.setTimeF(value); break;
            case "G": status = trafficLight.setTimeG(value); break;
            case "H": status = trafficLight.setTimeH(value); break;
            default: return ResponseEntity.badRequest().body(new MessageResponse("No timing value for " + timing));
        }
        if(status == 0) return ResponseEntity.ok(new MessageResponse(""));
        else return ResponseEntity.badRequest().body(new MessageResponse("Timing value could not be set!"));
    }

    private static final Object monitor = new Object();
    private int counter = 0;
    private class Test extends Thread {
        String status = "In progress";
        final Set<String> whatToTest;
        final int id;

        private Test(Set<String> whatToTest) {
            this.whatToTest = whatToTest;
            int2tests.put((id = counter++), this);
            this.start();
        }

        @Override
        public void run() {
            synchronized (monitor) {
                String state = trafficLight.getState();
                trafficLight.setState(TrafficLight.getStates().get(0));
                for (String s : whatToTest) {
                    try {
                        Thread.sleep(1000);
                    } catch (InterruptedException e) {
                        e.printStackTrace();
                    }
                    switch (s) {
                        case "red":
                            if (trafficLight.testRed() == 0) {
                                logRepository.save(new Log("Red lamp failed" , 1));
                                status = "Red lamp failed";
                                return;
                            }
                            break;
                        case "ylw":
                            if (trafficLight.testYlw() == 0) {
                                logRepository.save(new Log("Yellow lamp failed" , 1));
                                status = "Yellow lamp failed";
                                return;
                            }
                            break;
                        case "grn":
                            if (trafficLight.testGrn() == 0) {
                                logRepository.save(new Log("Green lamp failed" , 1));
                                status = "Green lamp failed";
                                return;
                            }
                            break;
                        case "pred":
                            if (trafficLight.testPred() == 0) {
                                logRepository.save(new Log("Pedestrian red lamp failed" , 1));
                                status = "Pedestrian red lamp failed";
                                return;
                            }
                            break;
                        case "pgrn":
                            if (trafficLight.testPgrn() == 0) {
                                logRepository.save(new Log("Pedestrian green lamp failed" , 1));
                                status = "Pedestrian green lamp failed";
                                return;
                            }
                            break;
                        default:
                            logRepository.save(new Log("No tests available for '" + s + "'" , 2));
                            status = "No tests available for '" + s + "'";
                            trafficLight.setState(state);
                            return;
                    }
                }
                logRepository.save(new Log("Testing was successful" , 0));
                status = "Test OK";
                trafficLight.setState(state);
            }
        }
    }



}
