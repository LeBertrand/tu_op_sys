#include SOFAT.h

/*	In order to make dependency hierarchy simple, this will use no
	member methods of File object. All functionality explicitly
	implemented here. */
blockID SOFAT_allocate_block(blockID predecessor)
{
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
	fprintf(stderr, "Error: Memory Full.\n");
	exit(1);
}