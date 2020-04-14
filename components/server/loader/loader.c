#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define MSGBOX_BASE         0x40000000
#define CORE3_MSGBOX_SET    0x000000bc
#define CORE3_MSGBOX_CLR    0x000000fc
#define BASE_ADDR           0x20000000

int main (int argc, char * argv [])
{
    int flag = -1;
    int fd_mem;
    void *load_address;
    unsigned long len;
    fd_mem = open("/dev/mem", O_RDWR);
    void* msgboxes = mmap(NULL,4096, PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, MSGBOX_BASE);
    volatile unsigned int* msgbox_set = msgboxes + CORE3_MSGBOX_SET;
    volatile unsigned int* msgbox_clr = msgboxes + CORE3_MSGBOX_CLR;
    if(msgbox_clr == (void*)-1 || msgbox_set == (void*)-1) {
        printf("Not mappable: %s (%d, %d)\n", strerror(errno), msgbox_clr, msgbox_set);
    }
    else {
    printf("(%x, %x)\n", msgbox_clr, msgbox_set);
        if(*msgbox_clr) {
            printf("You cannot interrupt a running program. %x\n", *msgbox_clr);
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
                    printf("Successfully loaded %d bytes. Mailbox now reads: 0x%x\n", len, *msgbox_clr);
                    flag = 0;
                }
                munmap(load_address, len);
            }
            fclose(file);
        }
        close(fd_mem);
    }
    munmap(msgboxes, 4096);
    return flag;
}
