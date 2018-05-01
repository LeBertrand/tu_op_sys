#include "SOFAT.h"

int open_file(char *path){
	// Create SOFILE to navigate given path.
	SOFILE *navfile = (SOFILE *) malloc(sizeof(SOFILE));
	// Set new SOFILE to root.
	navfile->abs_path[0] = '\0';
	navfile->first_SOFAT_entry = 0;
	navfile->bytes_written = 0;
	navfile->phys_start_loc = block_byte_map(blocks_offset, 0);
	navfile->current_position = 0;

	// Tokenize input
	char *cur_tok = strtok(path, "/");
	while( cur_tok != NULL ){
		printf("%s\n", cur_tok);
		cur_tok = strtok(NULL, "/");
	}

	return 0;
}
