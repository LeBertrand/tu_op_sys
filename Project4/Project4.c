#include <SOFAT.h>

short block_byte_map(blockID block, unsigned char page_offset){
	return blocksize * block + page_offset;
}

blockID get_successor_FAT_entry(blockID predecessor)
{
	blockID successor = *(start_of_FAT + predecessor*sizeof(blockID));
	return successor==0xffff?-1:successor;
}

char *linux_SOFAT_addr_translate(char *linux_virt_address){
	// Ugliest workaround ever, but code compiles.
	// Was getting type error, even though it's all char *.
	long int addr = (long int) linux_virt_address;
	addr -= (long int) physical_memory;
	return (char*) addr;
}

void boot_process(){

    int fd = open("Drive2MB",  O_RDWR | O_CREAT, NULL);
    if(fd < 0){
        puts("Open failed. Cannot open physical memory.");
        exit(1);
    }

	// Try to map to drive
	physical_memory = (char *) mmap(NULL, 0x200000, PROT_READ | PROT_WRITE | PROT_EXEC, MAP_SHARED, fd, 0);
	if(physical_memory == MAP_FAILED){
		puts("Map Failed. Cannot map to file system physical memory.");
	}

	// TODO: Replace hard coded stuff with reading the boot sector.
	FATblocks = 0x10; // 16 blocks are the FAT
	rootblocks = 8;
	blocks_offset = rootblocks + FATblocks + bootblocks;

	/***************************************************
	*	Convenience pointers: Start locations of sectors
	***************************************************/
	
	// Set memory pointer to start of FAT for searching FAT entries.
	start_of_FAT = (blockID*) &(physical_memory[bootblocks * blocksize]);
	// Set memory pointer to start of root for traversing file tree.
	start_of_root = &( physical_memory[ (bootblocks + FATblocks) * blocksize]);
	end_of_FAT = (blockID*) start_of_root - sizeof(blockID);

	//working_directory = new DirectoryListObject();
	working_directory = (SOFILE*) malloc(sizeof(SOFILE));
}

blockID SOFAT_allocate_block(blockID predecessor)
{
	// Create pointer for current search FAT entry
	blockID *blocksearch = start_of_FAT;
	/* Skip block zero. Begin indexing blocks at 1, so multiplication on pointers
	maps correctly. */
	blockID new_block_number = 1;

	// Advance blocksearch to point to FAT Entry of free block
	while( blocksearch <= end_of_FAT ){
		if( *blocksearch == 0 ) break; // Found free block
		// Point blocksearch to next FAT Entry
		blocksearch++;// += sizeof(blockID);
		new_block_number++;
	}/* Not clear on how while-else works in C++, but it seems like now's the time for it */

	// If search covered whole FAT, all memory is addressed.
	if(blocksearch > end_of_FAT){
		fprintf(stderr, "Error: Memory Full.\n");
		exit(1);
	} // post-condition: Found a free block.

	// If argument was given, adjust predecessor's FAT entry to point to new block.
	if(predecessor >= 0 ){
		// Extending existing file. Record extension in predecessor FAT entry.
		blockID *pred_block_pointer =  start_of_FAT + predecessor * sizeof(blockID);
		*pred_block_pointer = new_block_number;
	}
	*blocksearch = 0xffff;
	
	return new_block_number;
}

void SOFILE_teardown(SOFILE *sofile)
{
	free(sofile->abs_path);
	free(sofile);

	return;
}

void DirectoryListing_teardown(DirectoryListing *dir)
{
	free(dir->filename);
	free(dir);

	return;
}

void write_directory_to_position(DirectoryListing *newdir, int *phys_mem_pos)
{
	// Write filename in first 100 bytes.
	strcpy( &( physical_memory[*phys_mem_pos] ), newdir->filename);

	// Advance to location for lock;
	*phys_mem_pos += PATH_LEN_MAX;

	// Write info bits to next three bytes
	physical_memory[(*phys_mem_pos)++] = newdir->lock;

	physical_memory[(*phys_mem_pos)++] = newdir->modified;

	physical_memory[(*phys_mem_pos)++] = newdir->subdirectory;

	// Get pointer to write to two bytes of memory.
	blockID *starting_block_loc = (blockID*) &(physical_memory[*phys_mem_pos]);
	*starting_block_loc = newdir->starting_block;
	// Move phys_mem_pos past starting block field.
	*phys_mem_pos += sizeof(blockID);

	// Set int point to size field to write to four bytes. 
	int *size_loc = (int*) &(physical_memory[*phys_mem_pos]);
	*size_loc = newdir->size;
	// Move phys_mem_pos past size field. 
	*phys_mem_pos += sizeof(int);
}

DirectoryListing *read_directory_from_position(int *phys_mem_pos)
{
	DirectoryListing *readdir = (DirectoryListing*) malloc(sizeof(DirectoryListing));

	readdir->filename = strdup( &physical_memory[*phys_mem_pos] );
	*phys_mem_pos += PATH_LEN_MAX;

	readdir->lock = physical_memory[(*phys_mem_pos)++];
	readdir->modified = physical_memory[(*phys_mem_pos)++];
	readdir->subdirectory = physical_memory[(*phys_mem_pos)++];
	
	// Set blockID pointer to starting block to read in two bytes.
	blockID *starting_block_loc = (blockID*) &physical_memory[*phys_mem_pos];
	readdir->starting_block = *starting_block_loc;
	// Move phys_mem_pos past starting block field.
	*phys_mem_pos += sizeof(blockID);
	
	// Set int point to size field to read in four bytes.
	int *size_loc = (int*) &(physical_memory[*phys_mem_pos]);
	readdir->size = *size_loc;
	// Move phys_mem_pos past size field. 
	*phys_mem_pos += sizeof(int);
	

	return readdir;
}