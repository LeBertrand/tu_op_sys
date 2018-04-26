#include "../SOFAT.hh"

#define dlisting "filename|signal message visible"
int main()
{
	boot_process();

	// char dlisting[35];
	// strcpy(dlisting, "filename|signal message visible");

	blockID newfileloc = SOFAT_allocate_block(0);

	int mem_offset = block_byte_map(blocks_offset + newfileloc, 0);

	strncpy( &physical_memory[mem_offset], dlisting, sizeof(dlisting));
}