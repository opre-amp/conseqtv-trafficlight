package hu.conseqtv.serverlogic.controllers;

import hu.conseqtv.serverlogic.payload.request.MessageRequest;
import hu.conseqtv.serverlogic.payload.response.MessageItemResponse;
import hu.conseqtv.serverlogic.payload.response.MessageResponse;
import hu.conseqtv.serverlogic.security.services.UserDetailsImpl;
import hu.conseqtv.serverlogic.storage.messageboard.Message;
import hu.conseqtv.serverlogic.storage.messageboard.MessageRepository;
import hu.conseqtv.serverlogic.storage.roles.ERole;
import hu.conseqtv.serverlogic.storage.roles.Role;
import hu.conseqtv.serverlogic.storage.users.UserRepository;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.ResponseEntity;
import org.springframework.security.access.prepost.PreAuthorize;
import org.springframework.security.core.context.SecurityContextHolder;
import org.springframework.web.bind.annotation.*;

import javax.validation.Valid;
import java.util.List;
import java.util.stream.Collectors;

@RestController
@RequestMapping("/messages")
public class MessageBoardController {

    @Autowired
    MessageRepository messageRepository;
    @Autowired
    UserRepository userRepository;

    @GetMapping("/all")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public List<MessageItemResponse> allMessages() {
        return messageRepository.findAll(getCurrentRole()).stream().map(MessageItemResponse::new).collect(Collectors.toList());
    }

    @GetMapping("/filtered")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public List<MessageItemResponse> filteredMessages(@Valid @RequestParam long since) {
        return (messageRepository.findAllWithCreationDateTimeAfter(since, getCurrentRole())).stream().map(MessageItemResponse::new).collect(Collectors.toList());
    }

    @PostMapping("/")
    @PreAuthorize("hasRole('USER') or hasRole('POLICE') or hasRole('ADMIN')")
    public ResponseEntity<?> addMessage(@Valid @RequestBody MessageRequest message) {
        messageRepository.save(new Message(message.getUsername(), message.getMessage(), System.currentTimeMillis(), message.getERole()));
        return ResponseEntity.ok(new MessageResponse("Message saved"));
    }

    private ERole getCurrentRole() {
        Object principal = SecurityContextHolder.getContext().getAuthentication().getPrincipal();
        if(principal instanceof UserDetailsImpl)
            return ((Role) userRepository.findByUsername(((UserDetailsImpl) principal).getUsername()).get().getRoles().toArray()[0]).getName();
        else return ERole.ROLE_USER;
    }
}