
#ifndef BOOLEAN_T
#define BOOLEAN_T
typedef enum { false = 0, true = 1 } boolean_t;
#endif


#ifndef PROJECT4H
#define PROJECT4H

typedef struct {
	char filename[100],
	boolean_t lock,
	boolean_t subdirectory,
	short starting_block,
	int size
} DirectoryListing;

// Currently working with 2^12 blocks
typedef short blockID;

/*
 * Function: SOFAT_allocat_block
 * Input:
 	predecessor - blockID of the block to be extended, or 0
 * Return - blockID of newly allocated block.
 */
blockID SOFAT_allocate_block(blockID predecessor);

class DirectoryListObject {

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
		blockID; parent_starting_block;
		// Bytes written since last update to DirectoryListing
		long bytes_written;

} ;

#endif