#include <SOFAT.h>
#include <Project4.c>
#include <Project4API.h>

SOFILE *open_file(const char *path, SOFILE *wd){

	SOFILE *opened = (SOFILE*) malloc(sizeof(SOFILE));
	// TODO: Decent constructor for blank SOFILE
	opened->abs_path = malloc(PATH_LEN_MAX);

	/******* Base Case: Open Root Directory as File Object *********/
	if( !strcmp(path, "\0") || !strcmp(path, "~") || !strcmp(path, "ROOT")){
		strcpy(opened->abs_path, "~");
		// Can't point to SOFAT entry in root. Mark special case.
		opened->first_SOFAT_entry = -1;
		opened->bytes_written = 0;
		opened->phys_start_loc = block_byte_map(bootblocks + FATblocks, 0);
	}
	
	
	// TODO: Needs cd function implelemted to procede.
	SOFAT_cd(strtok(path, "/"), wd);
	open_file(strchr(path, wd));
	
	return opened;
}