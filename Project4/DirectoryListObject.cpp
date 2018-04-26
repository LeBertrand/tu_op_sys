#include "DirectoryListObject.hh"

DirectoryListObject::DirectoryListObject( /*const char *dirname */ )
{
	// Set start of current directory to start of root. Subtract root from offset to find root.
	data_block = -rootblocks;
	// Root doesn't have parent. Set parent to root.
	parent_starting_block = 0;

	current_position = (bytes_written = -1);

	in_directory = true;
}

/*
	if(!strcmp(dirname, "~") || !strcmp(dirname, "root") ||
		!strcmp(dirname, "HOME")){
		



	}
}
*/

void DirectoryListObject::advance_position()
{

}

void DirectoryListObject::backtrack()
{

}

bool DirectoryListObject::change_directory( char *directory_name )
{
	return true;
}

DirectoryListObject *DirectoryListObject::open_file(char *name, ...)
{
	return new DirectoryListObject();
}

bool DirectoryListObject::create_file(char *pathname, ...)
{
	return true;
}

void DirectoryListObject::create_directory(char *pathname, ...)
{

}

void DirectoryListObject::close_file()
{

}