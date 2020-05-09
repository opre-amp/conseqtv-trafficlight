package bme.hu.hf;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;
import org.springframework.boot.autoconfigure.security.servlet.SecurityAutoConfiguration;
import org.springframework.security.crypto.password.PasswordEncoder;

@SpringBootApplication(exclude = {SecurityAutoConfiguration.class})
public class HfApplication implements CommandLineRunner {
	@Autowired
	private UserRepository repo;
	@Autowired
	private PasswordEncoder pe;

	public static void main(String[] args)
	{
		SpringApplication.run(HfApplication.class, args);
	}

	@Override
	public void run(String... args) throws Exception {
		User user = new User();
		user.setUsername("admin");
		user.setPassword(pe.encode("admin"));
		user.setRole("ADMIN_ROLE");

		repo.save(user);
	}
}
