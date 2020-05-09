package bme.hu.hf;

import bme.hu.hf.mailbox.TrafficLight;
import org.springframework.security.crypto.bcrypt.BCryptPasswordEncoder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.*;

import java.util.List;

@RestController
public class UserController {
    private final UserRepository repo;
    private TrafficLight tl;

    public UserController(UserRepository userRepository) {
        repo = userRepository;
    }

    @GetMapping("/users")
    List<User> getAll(){
        return repo.findAll();
    }

    @GetMapping("/state")
    String getState() {
        return tl.getState();
    }

    @GetMapping("/sensors/signal")
    boolean getPedestrianSignal() {
        //derítsd ki, be van-e kapcsolva a gyalogos lámpa
        return false;
    }

    @GetMapping("/sensors/stopped")
    boolean getCarSensorSignal() {
        //derítsd ki, be van-e kapcsolva a szenzor
        return false;
    }

    @GetMapping("/tests/{id}")
    String getTestState(@PathVariable Long id) {
        //szerezd meg az adott teszt állapotát
        return "not imlemented yet";
    }

    @GetMapping("/times")
    int[] getTimes() {
        //How to get the timings?
        return null;
    }

    @PostMapping("/login")
    User login(@RequestParam("user") String username, @RequestParam("password") String pwd){
        return null;
    }

    @PostMapping("/users")
    User newUser(@RequestBody String name, @RequestBody String pass, @RequestBody String lvl){

        PasswordEncoder pe = new BCryptPasswordEncoder();
        User user = new User();
        user.setUsername(name);
        user.setPassword(pe.encode(pass));
        user.setRole(lvl);

        return repo.save(user);
    }

    @PostMapping("/input/police")
    void policeSignal(){
        tl.sendSignal(TrafficLight.getSignals().get(2));
    }

    @PostMapping("/input/switch")
    void switchOnOff(){
        if(tl.getState().equals("Off"))
            tl.sendSignal(TrafficLight.getSignals().get(0));
        else
            tl.sendSignal(TrafficLight.getSignals().get(1));
    }

    @PostMapping("/tests")
    Long createTest(){
        //Új teszt létrehozása és elindítása
        return null;
    }

    @PutMapping("/users/{name}")
    User updateUser(@RequestBody User user, @PathVariable String name) {

        return repo.findById(name)
                .map(usr -> {
                    usr.setUsername(user.getUsername());
                    usr.setPassword(user.getPassword());
                    usr.setRole(user.getRole());
                    return repo.save(usr);
                })
                .orElseGet(() -> {
                    user.setUsername(name);
                    return repo.save(user);
                });
    }

    @PutMapping("/state")
    void setState(@RequestBody String state){
        tl.setState(state);
    }

    @PutMapping("/program")
    void setProgram(@RequestBody String program){
        //do it later
    }

    @PutMapping
    void setTimings(@RequestBody int[] times){
        tl.setTimeA(times[0]);
        tl.setTimeB(times[1]);
        tl.setTimeC(times[3]);
        tl.setTimeC_(times[4]);
        tl.setTimeD(times[5]);
        tl.setTimeE(times[6]);
        tl.setTimeF(times[7]);
        tl.setTimeG(times[8]);
        tl.setTimeH(times[9]);
    }

    @DeleteMapping("/users/{name}")
    void deleteUser(@PathVariable String name){
        repo.deleteById(name);
    }
}
