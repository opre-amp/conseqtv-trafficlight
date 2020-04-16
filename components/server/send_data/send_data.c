#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <signal.h>

typedef struct 
{
    char string[255];
    int len;
} sstring;

#define WRITE_SSTRING _IOW('c','l',sstring*)

int fd = -1;

void intHandler(int _i) {
    printf("Closing Driver\n");
    if(fd >= 0) close(fd);
    exit(0);
}

int main(int argc, char** argv)
{
    int32_t value, number;
    sstring buf;
    signal(SIGINT, intHandler);
    if(argc <= 1) {
        printf("No parameters given, exiting.\n");
        return 0;
    }
    printf("\nOpening mbox file\n");
    fd = open("/dev/mbox", O_RDWR);
    if(fd < 0) {
        printf("Cannot open mbox file, exiting.\n");
        return -1;
    }
    strcpy(buf.string, argv[0]);
    buf.len = strlen(buf.string)+1;
    ioctl(fd, WRITE_SSTRING, (sstring*) &buf); 
    printf("Successfully sent %s\n.", buf.string);
    return 0;
} 