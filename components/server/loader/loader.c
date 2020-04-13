#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>

int main (int argc, char * argv [])
{
    int fd_mem;
    void *load_address;
    unsigned long len;
    FILE *file = fopen(argv[1],"rb");

    fseek(file, 0, SEEK_END);
    len=ftell(file);
    fseek(file, 0, SEEK_SET);

    fd_mem = open("/dev/mem", O_RDWR);
    load_address = mmap(NULL,len,PROT_READ|PROT_WRITE, MAP_SHARED, fd_mem, 0x20000000);

    fread(load_address, len, 1, file);
    fclose(file);
}
