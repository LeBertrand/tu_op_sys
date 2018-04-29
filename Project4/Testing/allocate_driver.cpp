#include "../SOFAT.hh"

/*
 *	Because of difficulty compiling with many files and circular
 *	dependencies, I'm copying and pasting functions into drivers' files.
 */

const char *dlisting = "filename|signal message visible";


blockID get_successor_FAT_entry(blockID predecessor)
{
	blockID successor = *(start_of_FAT + predecessor*sizeof(blockID));
	return successor==0xffff?-1:successor;
}

int main()
{
	boot_process();

	// char dlisting[35];
	// strcpy(dlisting, "filename|signal message visible");

	blockID newfileloc = SOFAT_allocate_block(-1);

	int mem_offset = block_byte_map(blocks_offset + newfileloc, 0);
	/* DBRL */
	printf("Calculated Physical Address %d / %x\n", mem_offset, mem_offset);
	strncpy( &physical_memory[mem_offset], dlisting, strlen(dlisting) - 1);

	blockID secondblock = SOFAT_allocate_block(newfileloc);
	mem_offset = block_byte_map(blocks_offset + secondblock, 0);

	printf("Calculated Physical Address %d / %x\n", mem_offset, mem_offset);
	strncpy( &physical_memory[mem_offset], dlisting, strlen(dlisting) - 1);

	puts("\nNow searching for block following the fifth (4)");
	printf("Found block %d.\n", get_successor_FAT_entry(4));
	puts("Now searching for block following the sixth (5)");
	printf("Found block %d.\n", get_successor_FAT_entry(5));
	return 0;
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
	rootblocks = 2;
	blocks_offset = rootblocks + FATblocks + bootblocks;

	/***************************************************
	*	Convenience pointers: Start locations of sectors
	***************************************************/
	
	// Set memory pointer to start of FAT for searching FAT entries.
	start_of_FAT = (blockID*) &(physical_memory[bootblocks * blocksize]);
	// Set memory pointer to start of root for traversing file tree.
	start_of_root = &( physical_memory[ (bootblocks + FATblocks) * blocksize]);
	end_of_FAT = (blockID*) start_of_root - sizeof(blockID);

	working_directory = new DirectoryListObject();

}

blockID SOFAT_allocate_block(blockID predecessor)
{
	blockID *blocksearch = start_of_FAT;
	blockID new_block_number = 1;
	// Advance blocksearch to point to FAT Entry of free block
	while( blocksearch <= end_of_FAT ){
		if( *blocksearch == 0 ) break; // Found free block
		// Point blocksearch to next FAT Entry
		blocksearch++;// += sizeof(blockID);
		new_block_number++;
	}/* Not clear on how while-else works in C++, but it seems like now's the time for it */
	if(blocksearch > end_of_FAT){
		fprintf(stderr, "Error: Memory Full.\n");
		exit(1);
	} // Found a free block.
	if(predecessor >= 0 ){
		// Extending existing file. Record extension in predecessor FAT entry.
		int predecessor_FAToffset = predecessor * sizeof(blockID);
		blockID *pred_block_pointer =  start_of_FAT + predecessor * sizeof(blockID);
		*pred_block_pointer = new_block_number;

		/* DBRL */
		printf("FAT Entry number %d, writing 0x%x to location %d / 0x%x.\n", 
			predecessor, new_block_number,
			linux_SOFAT_addr_translate( (char*) start_of_FAT) + predecessor_FAToffset,
			linux_SOFAT_addr_translate( (char*) start_of_FAT) + predecessor_FAToffset
			);
	}
	/* DBRL */	
	printf("FAT Entry number %d, writing 0x%x to location %d / 0x%x.\n", 
			new_block_number,
			0xffff,
			linux_SOFAT_addr_translate( (char*) blocksearch ),
			linux_SOFAT_addr_translate( (char*) blocksearch )
			);
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
