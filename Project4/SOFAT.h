
/*
 *	Project 4 Header Internal Functions File
 *	Shmuel's OS File Allocation Table - File System Implementation
 *	Temple University, Intro to Operating Systems, Spring 2018

 *	Functions to be used in file management by File System. Below functions are for internal use.
 *	See API header for abstracted file system functions.
 
 *	This project may show traces of an earlier C++ implementation that's been removed.
 	I lost my patience with C++ and switch back to C.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif


#ifndef PROJECT4H
#define PROJECT4H

// Number of blocks before block numbering begins
static const char bootblocks = 1; // Only one block ever given to boot info.
static char blocks_offset = 0; // Statically initialize to 0, but reset in boot procedure
static char FATblocks = 0; // ' '
static char rootblocks = 0; // ' '
static char MAX_BLOCKID = 212;

// Currently working with 2^12 blocks
typedef short blockID;
static const unsigned short blocksize = 512;
static const int MB_SIZE = 0x100000;
static const char SEOF = -2;
static const unsigned char PATH_LEN_MAX = 0x77;

// Convenience pointer to start of FAT for searching FAT entries.
static blockID *start_of_FAT, *end_of_FAT;
// Convenience pointer to start of root for traversing file tree.
static char *start_of_root;

// Global scope for pointer to memory map buffer
static char *physical_memory;

/* Data struct used to write directories to data area.
Functions gather necessary file or subdirectory info in struct and
then write it to data area of parent directory.

Potentially, one byte of status bits could be developed to shrink
struct by two bytes, but that's low priority, since space isn't an issue
and design would be more complicated. */
typedef struct DirectoryListing {
	char *filename;
	boolean_t lock;
	boolean_t modified;
	boolean_t subdirectory;
	blockID starting_block;
	int size;
} DirectoryListing;

const int DirectoryListingLen = 0x80;
const int DL_StartingBlockOffset = 0x7a;

/* Struct used by API functions for reading and manipulation of data
according to logic of filesystem rather than physical location.
Don't try to use as POSIX FILE struct. IO is implemented using these,
but the POSIX equivalent is dirent. */

typedef struct {
	char *abs_path;
	blockID first_SOFAT_entry;
	unsigned short bytes_written;
	long int phys_start_loc;
	long int current_position;
	blockID parentdirectory; // Block number of parent.
} SOFILE ;

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
short block_byte_map(blockID block, unsigned char page_offset);

/*
 *	Function: Linux_SOFAT_translate
 *	Input:
 	linux_virt_address - Memory pointer, really a virtual address
 	from Linux program running SOFAT.
 *	Return - physical address on drive addressed by SOFAT
 *	Use function to reduce human confusion in checking
 *	
 */
char *linux_SOFAT_addr_translate(char *linux_virt_address);

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

/*
 *	Function: SOFILE_teardown
 *	Input:
 	file - struct to destroy

 *	Free all memory used by struct and components.
 */
void SOFILE_teardown(SOFILE *file);

/*
 *	Function: DirectoryListing_teardown
 *	Input:
 	dir - struct to destroy

 *	Free all memory used by struct and components.
 */
void DirectoryListing_teardown(DirectoryListing *dir);

//#include "DirectoryListObject.cpp"

//DirectoryListObject *working_directory;
SOFILE *working_directory;

#endif