
/*
 *	Project 4 Header File
 *	Shmuel's OS File Allocation Table - File System Implementation
 *	Temple University, Intro to Operating Systems, Spring 2018

 *	Functions to be used in file management by File System and by applications.
 
 *	This project involves C++ because I determined that file objects with member
 	methods would be helpful for my own implementation of open file objects.
 	Otherwise, I've maintained consistency with C, using my own implementation
 	of the boolean type and character arrays insted of std:string objects.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>


// #ifndef BOOLEAN_T
// #define BOOLEAN_T
// enum class boolean_t { false = 0, true = 1 };
// #endif
// Don't know why that enum won't compile, but I'll just use the C++ bool

#ifndef PROJECT4H
#define PROJECT4H

typedef struct DirectoryListing {
	char filename[100];
	bool lock;
	bool subdirectory;
	short starting_block;
	int size;
} DirectoryListing;

// Currently working with 2^12 blocks
typedef short blockID;
const unsigned short blocksize = 512;
const int MB_SIZE = 0x100000;
const char SEOF = -2;

// Number of blocks before block numbering begins
const char bootblocks = 1; // Only one block ever given to boot info.
char blocks_offset = 0; // Statically initialize to 0, but reset in boot procedure
char FATblocks = 0; // ' '
char rootblocks = 0; // ' '
char MAX_BLOCKID = 212;

// Convenience pointer to start of FAT for searching FAT entries.
blockID *start_of_FAT, *end_of_FAT;
// Convenience pointer to start of root for traversing file tree.
char *start_of_root;

// Global scope for pointer to memory map buffer
char *physical_memory;

/*
 * Function: block_byte_map
 * Input:
 	block - block number
 	offset - byte within block
 * Return physical address indicated by block number and byte offset

 * Convenience function to turn block number:offset pair into one
   physical address offset (number of bytes from start of
   physical memory).
 */
inline short block_byte_map(blockID block, unsigned char page_offset){
	return blocksize * block + page_offset;
}

/*
 *	Function: Linux_SOFAT_translate
 *	Input:
 	linux_virt_address - Memory pointer, really a virtual address
 	from Linux program running SOFAT.
 *	Return - physical address on drive addressed by SOFAT
 *	Use function to reduce human confusion in checking
 *	
 */
inline char *linux_SOFAT_addr_translate(char *linux_virt_address){
	// Ugliest workaround ever, but code compiles.
	// Was getting type error, even though it's all char *.
	long int addr = (long int) linux_virt_address;
	addr -= (long int) physical_memory;
	return (char*) addr;
}

/*
 * Function: SOFAT_allocate_block
 * Input:
 	predecessor - blockID of the block to be extended, or 0
 * Throws:
 	Memory Full Error
 * Return - blockID of newly allocated block.

 * Allocate new block of memory. If predecessor in range [1, 212],
 	find it in FAT, and set it to blockID of newly allocated block.
 	Set newly allocated block to 0xFFFF.
 	Return blockID of new block.
 */
blockID SOFAT_allocate_block(blockID predecessor);

/*
 *	Function: get_successor_FAT_entry
 *	Input:
 	predecessor - block to look up in FAT
 *	Return: -1 if predecessor is in fact end of file, else blockID
 	of block following predecessor.
 */
blockID get_successor_FAT_entry(blockID predecessor);

/*
 *	Function: boot_process
 *	Reads in values to determine blocks offset to data area.
	Create new DirectoryListObject for current working directory.
 */
void boot_process();

#endif

#include "DirectoryListObject.cpp"

DirectoryListObject *working_directory;
