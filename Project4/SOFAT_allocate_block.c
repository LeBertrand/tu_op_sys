#include "SOFAT.hh"

/*	In order to make dependency hierarchy simple, this will use no
	member methods of File object. All functionality explicitly
	implemented here. */

/**
 *	Linear search of FAT for unallocated block.

 *	Warning: If this function is used carelessly, and blocks are allocated but not
 	associated with file in directory listing, they're lost. Mistake will be hard to correct.
 	Only use this in a function that immediately writes a Directory Listing.
 */
blockID SOFAT_allocate_block(blockID predecessor)
{
	blockID *blocksearch = start_of_FAT;
	blockID new_block_number = 0;
	// Advance blocksearch to point to FAT Entry of free block
	while( blocksearch <= end_of_FAT ){
		if( *blocksearch == 0 ) break; // Found free block
		// Point blocksearch to next FAT Entry
		blocksearch += sizeof(blockID);
		new_block_number++;
	}/* Not clear on how while-else works in C++, but it seems like now's the time for it */
	if(blocksearch > end_of_FAT){
		fprintf(stderr, "Error: Memory Full.\n");
		exit(1);
	} // Found a free block.
	if(predecessor > 0 ){
		// Extending existing file. Record extension in predecessor FAT entry.
		*(start_of_FAT + predecessor) = new_block_number;
	}
	*blocksearch = 0xffff;
	/*
	blockID FATbuf[blocksize / sizeof(blockID)];
	// TODO: Map to second block;
	//blockID *searcher = blocksize;
	// Go to beginning of FAT;
	
	unsigned short index;
	for(index = 0; index < blocksize / sizeof(blockID; index++){
		if( FATbuf[index] == 0){
			// Found free block. Allocate it.
			if(predecessor > 0){
			// FAT[predecessor] = index;
			}
			return index;
		}
	}
	*/

	return new_block_number;	
}