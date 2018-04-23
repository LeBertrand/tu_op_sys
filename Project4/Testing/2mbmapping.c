#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

const int BLOCKSIZE = 512;

int main(int argc, char *argv[])
{
    int fd = open("Drive2MB",  O_RDWR | O_CREAT, NULL);
    if(fd == NULL){
        puts("Failed to open file.");
        exit(1);
    }

    //write(fd, "NUMS...\0\0\0\0...", 18);
    /*
    char *buf = (char *) mmap(-1, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    char i = 'A';
    while(i <= 'Z'){
        buf[i - 'A'] = i++;
    }

    puts(buf);
    //strcpy(buf, "Now is the time for all good men to come to the aid of their party");
    */

    //const long int MB_SIZE = pow(2,20);
    const int MB_SIZE = 0x100000;
    char *buf = mmap(NULL, 2*MB_SIZE, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
    if(buf = MAP_FAILED){
        puts("map failed");
        exit(1);
    }
    puts("\nMemory mapped. Pointer returned.\n");

    unsigned long int i;
    for(i = 0; i < 100; i++){
        buf[i] = i % 100;
    }

    strcpy(buf, "Now is the time for all good men to come to the aid of their party");

    //munmap(buf, MB_SIZE);

    close(fd);
    return 0;
}
