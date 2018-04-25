#include "SOFAT.hh"

void boot_process(){

    int fd = open("Drive2MB",  O_RDWR | O_CREAT, NULL);
    if(fd == NULL){
        puts("Open failed. Cannot open physical memory.");
        exit(1);
    }

	// Try to map to drive
	physical_memory = (char *) mmap(NULL, 0x200000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	if(physical_memory == MAP_FAILED){
		puts("Map Failed. Cannot map to file system physical memory.");
	}

	// TODO: Replace hard coded stuff with reading the boot sector.
	FATblocks = 0x10; // 16 blocks are the FAT
	rootblocks = 2;
	blocks_offset = rootblocks + FATblocks + bootblocks;

	working_directory = new DirectoryListObject();

}