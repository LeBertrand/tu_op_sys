//#include <SOFAT.h>
#include <Project4.c>

const char *humanrights = "endowed by their creator certain inalienable rights,\
and that among these are life, liberty and pursuit of happiness";

boolean_t dbg_write_directory_to_position(DirectoryListing*, int*);
void dbg_read_directory_from_position(int*);

int main()
{
	printf("PATH_LEN_MAX is set to %d, rather than %d\n\n\n", PATH_LEN_MAX, 0xf8);
	puts("First line of code.\n");
	boot_process();
	puts("Booted up successfully.\n");
	// Set read write position to start of root.
	int rw_pos = block_byte_map(bootblocks + FATblocks, 0);
	printf("Positioned read/write head to start of root at 0x%x.\n\n", rw_pos);

	DirectoryListing *dirs[3];
	char *names[3] = { strdup("firstdir"), strdup("seconddir"), strdup("thirddir")};
	char i;
	for(i = 0; i < 3; i++){
		dirs[i] = (DirectoryListing *) malloc(sizeof(DirectoryListing));
		dirs[i]->filename = names[i];
		dirs[i]->lock = false;
		dirs[i]->modified = false;
		dirs[i]->subdirectory = true;
		dirs[i]->size = 0;
		// leave starting blocks unset
	}
	puts("Created DirectoryListing objects.\n");
	// Find first block for subdirectory, and write its listing to root.
	blockID firstdblock = SOFAT_allocate_block(-1);
	puts("Allocated first block of memory\n");
	dirs[0]->starting_block = firstdblock;
	puts("Copied string pointer to DirectoryListing struct's filename field\n");
	puts("*********************************************************************\n");
	dbg_write_directory_to_position(dirs[0], &rw_pos);
	printf("Wrote first directory to root, setting starting block to %d.\n\n", firstdblock);

	puts("*********************************************************************\n");
	// Same for second
	blockID seconddblock = SOFAT_allocate_block(-1);
	dirs[1]->starting_block = seconddblock;
	dbg_write_directory_to_position(dirs[1], &rw_pos);
	printf("Wrote second directory to root, setting starting block to %d.\n\n", seconddblock);

	puts("*********************************************************************\n");
	// Same for third
	blockID thirdblock = SOFAT_allocate_block(-1);
	dirs[2]->starting_block = thirdblock;

	// Save location of third block so writing later doesn't need to find it. Shortcut not in real application.
	int third_dir_phy_loc = rw_pos;

	dbg_write_directory_to_position(dirs[2], &rw_pos);
	printf("Wrote third directory to root, setting starting block to %d.\n\n", thirdblock);

	puts("*********************************************************************\n");
	// Write file into third directory.
	rw_pos = block_byte_map(bootblocks + FATblocks + thirdblock, 0);
	DirectoryListing filelisting = {
		"textfile", false, false, false, SOFAT_allocate_block(-1), strlen(humanrights)
	};
	dbg_write_directory_to_position( &filelisting, &rw_pos );
	printf("Wrote file to third directory, setting starting block to %d.\n\n",
		filelisting.starting_block);
	// Update size using location saved when creating thirdir
	int *td_size_addr = (int*) &physical_memory[third_dir_phy_loc + 0x7c];
	*td_size_addr = DirectoryListingLen;
	printf("Wrote size 0x%x to physical location 0x%x, the size field of thirddir.\n\n",
		DirectoryListingLen, third_dir_phy_loc + 0x7c);

	// Write file data to file data block.
	rw_pos = block_byte_map(bootblocks + FATblocks + filelisting.starting_block, 0);
	strcpy( &physical_memory[ rw_pos ], humanrights);
	printf("Wrote text to file data block at location 0x%x\n\n", rw_pos);

	rw_pos += strlen(humanrights);
	physical_memory[rw_pos++] = SEOF;

	// Reset rw_pos to try reading in three directories from root.
	rw_pos = block_byte_map(bootblocks + FATblocks, 0); 

	puts("At beginning of root directory. Reading...\n\n\n");

	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);

	// Reset rw_pos
	rw_pos = block_byte_map(bootblocks + FATblocks, 0);

	/*** Try to find textfile and read it out ***/

	// Linear search for directory named "thirddir"
	int endsearchat = block_byte_map(blocks_offset, 0);
	while( strcmp( &physical_memory[rw_pos], "thirddir") &&
		(rw_pos < endsearchat) ){
		printf("Reading through listings in root. Current position - 0x%x, %d until end of search area.\n",
			rw_pos, endsearchat - rw_pos);
		printf("Reading directory name %s.\n\n", &physical_memory[rw_pos]);
		rw_pos += DirectoryListingLen;
	} 
	if(rw_pos >= blocks_offset*blocksize){
		fprintf(stderr, "Root is full. Logic error spacing directory listings.");
		exit(1);
	} // Post: hit end of root or found open list spot
	printf("Found listing at 0x%x.\n", rw_pos);
	/* Cast array location to blockID pointer to read two bytes. Offset by known
	length of DirectoryListing until startingblock. */

	// Move up through listing to find starting block.
	rw_pos += DL_StartingBlockOffset;

	// Set blockID pointer to read out two bytes.
	blockID *block_finder = (blockID*) &physical_memory[rw_pos];
	printf("Found file starting_block %d at location 0x%x\n\n", *block_finder, rw_pos);

	// Go to data block indicated.
	rw_pos = block_byte_map(rootblocks + FATblocks + *block_finder, 0);
	printf("Attempting read of block at physical location 0x%x / %d\n\n", rw_pos, rw_pos);
	while( physical_memory[rw_pos] != SEOF){
		putchar(physical_memory[rw_pos++]);
	}
	puts("\nCompleted read of file");
	
	return 0;
}

boolean_t dbg_write_directory_to_position(DirectoryListing *newdir, int *phys_mem_pos)
{
	// Write filename in first 100 bytes.
	printf("Copying directory filename %s to physical_memory location 0x%x\n\n",
		newdir->filename, *phys_mem_pos);
	strcpy( &( physical_memory[*phys_mem_pos] ), newdir->filename);
	// Advance to location for lock;
	printf("Copied.\n\n");
	*phys_mem_pos += PATH_LEN_MAX;
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	// Write info bits to next three bytes
	physical_memory[(*phys_mem_pos)++] = newdir->lock;
	printf("Wrote lock %s\n\n", newdir->lock?"closed":"open");
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	physical_memory[(*phys_mem_pos)++] = newdir->modified;
	printf("Wrote modified bit %s\n\n", newdir->modified?"true":"false");
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	physical_memory[(*phys_mem_pos)++] = newdir->subdirectory;
	printf("Wrote subdirectory %s\n\n", newdir->subdirectory?"true":"false");
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	// Get pointer to write to two bytes of memory.
	blockID *starting_block_loc = (blockID*) &(physical_memory[*phys_mem_pos]);
	*starting_block_loc = newdir->starting_block;
	printf("Wrote starting block %d\n\n", newdir->starting_block);
	*phys_mem_pos += sizeof(blockID);
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	int *size_loc = (int*) &(physical_memory[*phys_mem_pos]);
	*size_loc = newdir->size;
	*phys_mem_pos += sizeof(int);
	printf("Wrote size %d\n\n", newdir->size);
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);

	return true;
}

void dbg_read_directory_from_position(int *phys_mem_pos)
{
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	printf("Read subdir name %s.\n", &physical_memory[*phys_mem_pos]);
	*phys_mem_pos += PATH_LEN_MAX;
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);

	// Create char* to read one byte at a time.
	////char *bits_finder = &physical_memory[ *phys_mem_pos];
	printf("Lock: %d, Modified: %d, Subdirectory: %d\n",
		physical_memory[(*phys_mem_pos)++],
		physical_memory[(*phys_mem_pos)++],
		physical_memory[(*phys_mem_pos)++]);
	////*phys_mem_pos += 3;
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	
	blockID *starting_block_loc = (blockID*) &physical_memory[*phys_mem_pos];
	printf("Starting Block: %d\n", *starting_block_loc);
	*phys_mem_pos += sizeof(blockID);
	
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	int *size_loc = (int*) &(physical_memory[*phys_mem_pos]);
	printf("Size of directory: %d\n", *size_loc);
	*phys_mem_pos += sizeof(int);
	
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);

	return;
}