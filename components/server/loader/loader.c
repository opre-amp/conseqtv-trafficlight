#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

#define CORE3_MSGBOX_SET    0x400000bc
#define CORE3_MSGBOX_CLR    0x400000fc
#define BASE_ADDR           0x20000000

int main (int argc, char * argv [])
{
    int flag = -1;
    int fd_mem;
    void *load_address;
    unsigned long len;
    fd_mem = open("/dev/mem", O_RDWR);
    volatile unsigned int* msgbox_set = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, CORE3_MSGBOX_SET);
    volatile unsigned int* msgbox_clr = mmap(NULL,4,PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, CORE3_MSGBOX_CLR);


    if(*msgbox_clr) {
        printf("You cannot interrupt a running program.\n");
    }
    else {
        FILE *file = fopen(argv[1],"rb");
        if(!file) {
            printf("Could not open file.\n");
        }
        else {
            fseek(file, 0, SEEK_END);
            len=ftell(file);
            fseek(file, 0, SEEK_SET);

            load_address = mmap(NULL,len,PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, BASE_ADDR);
            if(!load_address){
                printf("Memory could not be opened.\n");
            }
            else {
                fread(load_address, len, 1, file);
                *msgbox_set = BASE_ADDR;
                flag = 0;
            }
            munmap(load_address, len);
        }
        fclose(file);
    }
    munmap(msgbox_clr, 4);
    munmap(msgbox_set, 4);
    close(fd_mem);
    return flag;
}
