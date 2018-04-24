#include "SOFAT.hh"

void boot_process(){

	// Try to map to drive
	char *physical_memory = (char *) mmap(NULL, 0x200000);
	if(physical_memory == MAP_FAILED){
		puts("Map Failed. Cannot access file system physical memory.");
	}

	// TODO: Replace hard coded stuff with reading the boot sector.
	FATblocks = 0x10; // 16 blocks are the FAT
	rootblocks = 2;
	offset = rootblocks + FATblocks;

}