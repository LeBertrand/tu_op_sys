#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <math.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>

int main(int argc, char *argv[])
{
    int fd = open("Drive2MB",  O_RDWR | O_CREAT, NULL);
    if(fd == NULL){
        puts("Failed to open file.");
        exit(1);
    }

    write(fd, "File begins here.\n", 18);
    /*
    char *buf = (char *) mmap(-1, 1024, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    char i = 'A';
    while(i <= 'Z'){
        buf[i - 'A'] = i++;
    }

    puts(buf);
    //strcpy(buf, "Now is the time for all good men to come to the aid of their party");
    */

    const unsigned long int MB_SIZE = pow(2,20);
    char *buf = (char *) mmap(-1, MB_SIZE, PROT_READ | PROT_WRITE, MAP_PRIVATE, fd, 0);
    puts("\nMemory mapped. Pointer returned.\n");

    unsigned long int i;
    for(i = 0; i < 20; i++){
        buf[i] = i % 100;
    }

    munmap(buf, MB_SIZE);

    close(fd);
    return 0;
}
