
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
typedef unsigned short blockID;
const unsigned short blocksize = 512;
const int MB_SIZE = 0x100000;

// Number of blocks before block numbering begins
char blocks_offset = 0; // Statically initialize to 0, but reset in boot procedure
char FATblocks = 0; // ' '
char rootblocks = 0; // ' '

/*
 * Function: block_byte_map
 * Input:
 	block - block number
 	offset - byte within block
 * Return physical address indicated by block number and byte offset

 * Convenience function to turn block number:offset pair into one
   physical address.
 */
int block_byte_map(int block, int page_offset){
	return blocksize * block + page_offset;
}

/*
 * Function: SOFAT_allocat_block
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

class DirectoryListObject {

	public:
		/*
		 *	No arguments to constructor. Object starts at root by default.
		 *	All internal fields set to refer to root.
		 */
		DirectoryListObject();
		/*
		 * Function: advance_position

		 * Advance W/R position by one.
		 */
		void advance_position();

		/*
		 * Function: backtrack

		 * Retract W/R position by one.
		 */
		void backtrack();

		/*
		 *	Function: change_directory
		 *	Input:
		 	directoryname - name of subdirectory to navigate to
		 *	Return true if navigated to directory, else return false.

		 *	Change object to reflect a new directory. Map memory
		 	and update all fields to make that the working directory.
		 */
		bool change_directory(char *directory_name);	

		/*
		 *	Function: open_file
		 *	Input:
		 	name - filename in current directory, or absolute file path delimited by '/'
		 	options - flags
		 *	Return DirectoryListingObject set to named file.

		 *	Try to open file of given name by returning DirectoryListingObject
		 	to represent it. If argument is a single token, try to open as
		 	relative path. Else try absolute path from root. For file in root,
		 	~/filename works.
		 */
		static DirectoryListObject open_file(char *name, ...);

		/*
		 *	Function: create_file
		 *	Input:
		 	name - filename in current directory, or absolute file path delimited by '/'
		 	options - flags
		 *	Return true if file created, else false

		 *	Try to create file of given name. If argument is a single token, try to open as
		 	relative path. Else try absolute path from root. For file in root,
		 	~/filename works. Space allocation is handled. Function safe a application level.
		 */
		static bool create_file(char *pathname, ...);

		/*
		 *	Function: create_directory
		 *	Input:
		 	name - name of new director
		 	options - flags

		 *	Create new subdirectory in current directory, with given pathname.
		 */
		static void create_directory(char *pathname, ...);

		/*
		 *	Function: close_file
		 *
		 *	Close file currently represented by this object, and clean up object.
		 *	Unlock file, update info in directory listing, and update internal fields.
		 */
		static void close_file();

		/*
		 *	Function: write_file
		 *	Input: input_string - string to insert in file at current position

		 *	Return number of characters written

		 *	Write given string to file. Allocate space as necessay.
		 	Return number of bytes written, which in normal cases should be
		 	length of input_string.
		 */
		int write_file(char *input_string);

		/*
		 *	Function: read_file
		 *	Input: num_characters - Maximum number of characters to read from file
		 *	Write given string to file. Allocate space as necessay.
		 *	Return number of bytes written, which in normal cases should be
		 	length of input_string.

		 *	Read from file, a maximum of n characters, copy into buffer, and
		 	return location of buffer.
		 *	Read not supported on string length more than 2^8 
		 */
		char *read_file(long n);

		/*
		 *	Function: delete_file
		 *	Input: filename - local name or absolute path of file to delete
		 *	Return true if file found and deleted, else false
		 
		 *	Search for file with name passed. If file found, show its blocks
			as available, and clear its directory listing.
		 	
		 */
		bool delete_file(char *filename);
		 
		/*
		 *	Function: seek_position
		 *	Input: N - byte number target for position pointer
		 *	Return number of bytes read, up to N.
		 */
		long seek_position(long N);

	private:
		// Buffer mapped to current data block
		char open_memory[512];
		// Number of data block currently mapped to memory
		blockID data_block;
		// Pointer to physical location where current data block begins
		char *current_block_start;
		// Read/Write position in current block
		short current_position;
		// Pointer to parent starting block
		blockID parent_starting_block;
		// Bytes written since last update to DirectoryListing
		long bytes_written;
		
		
		// Map to new block
		bool remap(blockID);

} ;

DirectoryListObject working_directory;

#endif