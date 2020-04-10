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
#define READ_SSTRING  _IOR('c','z',char*)

int fd = -1;

void intHandler(int _i) {
    printf("Closing Driver\n");
    if(fd >= 0) close(fd);
}
 
int main()
{
    int32_t value, number;
    signal(SIGINT, intHandler);

    printf("\nOpening Driver\n");
    fd = open("/dev/mbox", O_RDWR);
    if(fd < 0) {
        printf("Cannot open device file...\n");
        return 0;
    }
    char buf[255];
    while(1) {
        while(ioctl(fd, READ_SSTRING, (char*) &buf) < 0) {
            usleep(10000);
        }
        printf("%s\n", buf);
    }
}