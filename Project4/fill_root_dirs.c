#include <SOFAT.h>
#include <Project4.c>

boolean_t dbg_write_directory_to_position(DirectoryListing*, int*);
void dbg_read_directory_from_position(int*);

int main()
{
	puts("First line of code.\n");
	boot_process();
	puts("Booted up successfully.\n");
	// Set read write position to start of root.
	int rw_pos = block_byte_map(blocks_offset, 0);
	puts("Positioned read/write head correctly.\n");

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
	dbg_write_directory_to_position(dirs[2], &rw_pos);
	printf("Wrote third directory to root, setting starting block to %d.\n\n", thirdblock);

	// Write file into third directory.
	rw_pos = block_byte_map(blocks_offset + thirdblock, SOFAT_allocate_block(-1), 0);
	DirectoryListing filelisting = { "textfile", false, false, false, 0 } ;
	dbg_write_directory_to_position( &filelisting, &rw_pos );
	printf("Wrote file to third directory, setting starting block to %d.\n\n",
		filelisting.starting_block);

	// Reset rw_pos to try reading in three directories from root.
	rw_pos = block_byte_map(blocks_offset, 0); 

	puts("At beginning of root directory. Reading...\n\n\n");

	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	puts("*********************************************************************\n");
	dbg_read_directory_from_position(&rw_pos);
	
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
	*phys_mem_pos += PATH_MAX;
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
	*phys_mem_pos += PATH_MAX;
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
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	printf("Starting Block: %d\n", *starting_block_loc);
	*phys_mem_pos += sizeof(blockID);
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);
	int *size_loc = (int*) &(physical_memory[*phys_mem_pos]);
	printf("Size of directory: %d\n", *size_loc);
	*phys_mem_pos += sizeof(int);
	printf("Read/Write position is now 0x%x\n\n", *phys_mem_pos);

	return;
}