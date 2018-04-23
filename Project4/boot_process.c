#include "SOFAT.h"

void boot_process(){

	// Try to map to drive
	char *physical_memory = (char *) mmap(NULL, 0x200000);

	// TODO: Replace hard coded stuff with reading the boot sector.
	FATblocks = 0x10; // 16 blocks are the FAT
	rootblocks = 2;
	offset = rootblocks + FATblocks;

}