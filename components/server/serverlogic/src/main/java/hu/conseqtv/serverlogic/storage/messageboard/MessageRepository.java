package hu.conseqtv.serverlogic.storage.messageboard;

import hu.conseqtv.serverlogic.storage.roles.ERole;
import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface MessageRepository extends JpaRepository<Message, Long> {
    @Query("select a from Message a where a.timestamp > :creationDateTime and a.role <= :role")
    List<Message> findAllWithCreationDateTimeAfter(
            @Param("creationDateTime") long creationDateTime,
            @Param("role") ERole role);

    @Query("select a from Message a where a.role <= :role")
    List<Message> findAll(@Param("role") ERole role);
}