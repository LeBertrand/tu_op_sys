/*
 *	Project 4 Header for API Functions 
 *	Shmuel's OS File Allocation Table - File System Implementation
 *	Temple University, Intro to Operating Systems, Spring 2018

 *	Functions to be used for accessing file system. Internal implementation of
 	memory mapping partly hidden in SOFAT.hh. Functions declared here may be used
 	by applications for file functionality.

 *	This project may show traces of an earlier C++ implementation that's been removed.
 	I lost my patience with C++ and switched back to C.
 */

/*
		 *	Function: open_file
		 *	Input:
		 	    path - string containing file path
		 	    working_directory - directory containing file, ignored if path is absolute
		 *	Try to open file of given name by returning SOFILE struct
		 	to represent it.
		 *	

		 	// TODO: If argument is a single token, try to open as
		 	relative path. Else try absolute path from root. For file in root,
		 	~/filename works.
		 */
SOFILE *open_file(const char *path, SOFILE *working_directory);