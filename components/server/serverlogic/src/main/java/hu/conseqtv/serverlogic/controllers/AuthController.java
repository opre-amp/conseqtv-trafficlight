package hu.conseqtv.serverlogic.controllers;

import hu.conseqtv.serverlogic.payload.request.DeleteRequest;
import hu.conseqtv.serverlogic.payload.request.LoginRequest;
import hu.conseqtv.serverlogic.payload.request.ModificationRequest;
import hu.conseqtv.serverlogic.payload.request.SignupRequest;
import hu.conseqtv.serverlogic.payload.response.JwtResponse;
import hu.conseqtv.serverlogic.payload.response.MessageResponse;
import hu.conseqtv.serverlogic.security.jwt.JwtUtils;
import hu.conseqtv.serverlogic.security.services.UserDetailsImpl;
import hu.conseqtv.serverlogic.storage.log.Log;
import hu.conseqtv.serverlogic.storage.log.LogRepository;
import hu.conseqtv.serverlogic.storage.roles.ERole;
import hu.conseqtv.serverlogic.storage.roles.Role;
import hu.conseqtv.serverlogic.storage.roles.RoleRepository;
import hu.conseqtv.serverlogic.storage.users.User;
import hu.conseqtv.serverlogic.storage.users.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.authentication.AuthenticationManager;
import org.springframework.security.authentication.UsernamePasswordAuthenticationToken;
import org.springframework.security.core.Authentication;
import org.springframework.security.core.GrantedAuthority;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.security.crypto.password.PasswordEncoder;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;
import java.util.HashSet;
import java.util.List;
import java.util.Optional;
import java.util.Set;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/users")
public class AuthController {
    @Autowired
    AuthenticationManager authenticationManager;

    @Autowired
    UserRepository userRepository;

    @Autowired
    LogRepository logRepository;

    @Autowired
    RoleRepository roleRepository;

    @Autowired
    PasswordEncoder encoder;

    @Autowired
    JwtUtils jwtUtils;

    @PostMapping("/signin")
    public ResponseEntity<?> authenticateUser(@Valid @RequestBody LoginRequest loginRequest) {
        logRepository.save(new Log("Started user authentication for " + loginRequest.getUsername(), 0));
        Authentication authentication = authenticationManager.authenticate(new UsernamePasswordAuthenticationToken(loginRequest.getUsername(), loginRequest.getPassword()));
        logRepository.save(new Log("Successfully authenticated " + loginRequest.getUsername() + ", generating token (not logging further)", 0));
        SecurityContextHolder.getContext().setAuthentication(authentication);
        String jwt = jwtUtils.generateJwtToken(authentication);

        UserDetailsImpl userDetails = (UserDetailsImpl) authentication.getPrincipal();
        List<String> roles = userDetails.getAuthorities().stream().map(GrantedAuthority::getAuthority).collect(Collectors.toList());

        return ResponseEntity.ok(new JwtResponse(jwt, userDetails.getId(), userDetails.getUsername(), roles));
    }

    @PutMapping("/modify")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> modifyUser(@Valid @RequestBody ModificationRequest modificationRequest) {
        logRepository.save(new Log("Trying to modify user " + modificationRequest.getUsername(), 0));
        Optional<User> user;
        if(!(user = userRepository.findByUsername(modificationRequest.getUsername())).isPresent()) {
            logRepository.save(new Log("User " + modificationRequest.getUsername() + " does not exist!", 2));
            return ResponseEntity.badRequest().body(new MessageResponse("User does not exist!"));
        }

        if(modificationRequest.getPassword() != null) {
            logRepository.save(new Log("Updating password of " + modificationRequest.getUsername(), 0));
            user.get().setPassword(encoder.encode(modificationRequest.getPassword()));
        }
        if(modificationRequest.getRole() != null) {
            logRepository.save(new Log("Trying to update role of user " + modificationRequest.getUsername(), 0));
            Role role;
            switch (modificationRequest.getRole()) {
                case "admin":
                    role = roleRepository.findByName(ERole.ROLE_ADMIN)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));

                    break;
                case "police":
                    role = roleRepository.findByName(ERole.ROLE_POLICE)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));

                    break;
                default:
                    role = roleRepository.findByName(ERole.ROLE_USER)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));
                    break;
            }
            user.get().getRoles().clear();
            user.get().getRoles().add(role);
            logRepository.save(new Log("Updated role of user " + modificationRequest.getUsername(), 0));
        }
        userRepository.save(user.get());
        logRepository.save(new Log("User " + modificationRequest.getUsername() + " modified successfully", 0));
        return ResponseEntity.ok(new MessageResponse("User modified successfully!"));
    }

    @DeleteMapping("/delete")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> deleteUser(@Valid @RequestBody DeleteRequest deleteRequest) {
        Optional<User> user;
        if((user = userRepository.findByUsername(deleteRequest.getUsername())).isPresent()) {
            if(userRepository.findAll().size() == 1) {
                return ResponseEntity.badRequest().body(new MessageResponse("Last user cannot be deleted!"));
            }
            userRepository.delete(user.get());
            return ResponseEntity.ok(new MessageResponse("User deleted successfully!"));
        }
        else {
            return ResponseEntity.badRequest().body(new MessageResponse("User does not exist!"));
        }
    }

    @PostMapping("/new")
    @PreAuthorize("hasRole('ADMIN')")
    public ResponseEntity<?> registerUser(@Valid @RequestBody SignupRequest signUpRequest) {
        logRepository.save(new Log("Trying to register user " + signUpRequest.getUsername(), 0));
        if (userRepository.existsByUsername(signUpRequest.getUsername())) {
            logRepository.save(new Log("User " + signUpRequest.getUsername() + " already exists", 2));
            return ResponseEntity
                    .badRequest()
                    .body(new MessageResponse("Error: Username is already taken!"));
        }

        handleUser(signUpRequest);
        logRepository.save(new Log("Successfully registered user " + signUpRequest.getUsername(), 0));

        return ResponseEntity.ok(new MessageResponse("User registered successfully!"));
    }

    private void handleUser(@RequestBody @Valid SignupRequest modificationRequest) {
        User user = new User(modificationRequest.getUsername(),
                encoder.encode(modificationRequest.getPassword()));

        String strRole = modificationRequest.getRole();
        Set<Role> roles = new HashSet<>();

        if (strRole == null) {
            Role userRole = roleRepository.findByName(ERole.ROLE_USER)
                    .orElseThrow(() -> new RuntimeException("Error: Role is not found."));
            roles.add(userRole);
        } else {
            switch (strRole) {
                case "admin":
                    Role adminRole = roleRepository.findByName(ERole.ROLE_ADMIN)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));
                    roles.add(adminRole);

                    break;
                case "police":
                    Role modRole = roleRepository.findByName(ERole.ROLE_POLICE)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));
                    roles.add(modRole);

                    break;
                default:
                    Role userRole = roleRepository.findByName(ERole.ROLE_USER)
                            .orElseThrow(() -> new RuntimeException("Error: Role is not found."));
                    roles.add(userRole);
            }
        }

        user.setRoles(roles);
        userRepository.save(user);
    }
}