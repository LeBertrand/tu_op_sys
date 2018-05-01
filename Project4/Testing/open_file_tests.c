#include "../SOFAT.h"
#include "../Project4.c"

SOFILE *open_file(const char *path){

	SOFILE opened = (SOFILE*) malloc(sizeof(SOFILE));
	// TODO: Decent constructor for blank SOFILE
	opened->abs_path = malloc(PATH_LEN_MAX);

	/******* Base Case: Open Root Directory as File Object *********/
	if( !strcmp(path, "\0") || !strcmp(path, "~") || !strcmp(path, "ROOT")){
		strncpy(opened->abs_path, "~");
	}
}