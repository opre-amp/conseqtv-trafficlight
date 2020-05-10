package hu.conseqtv.serverlogic.storage.log;

import org.springframework.data.jpa.repository.JpaRepository;
import org.springframework.data.jpa.repository.Query;
import org.springframework.data.repository.query.Param;
import org.springframework.stereotype.Repository;

import java.util.List;

@Repository
public interface LogRepository extends JpaRepository<Log, Integer> {
    @Query("select a from Log a where a.creationDateTime > :creationDateTime and a.logLevel > :minLogLevel")
    List<Log> findAllWithCreationDateTimeAfter(
            @Param("creationDateTime") long creationDateTime,
            @Param("minLogLevel") int logLevel);

    @Override
    List<Log> findAll();
}