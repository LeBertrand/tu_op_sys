#ifndef DIRECTORYLISTOBJECT_H
#define DIRECTORYLISTOBJECT_H

typedef short blockID;

class DirectoryListObject {

	public:
		/*
		 *	No arguments to constructor. Object starts at root by default.
		 *	All internal fields set to refer to root.
		 */
		DirectoryListObject( /*const char * */ );
		/*
		 * Function: advance_position

		 * Advance W/R position by one.
		 */
		void advance_position();

		/*
		 * Function: backtrack

		 * Retract W/R position by one.
		 */
		void backtrack();

		/*
		 *	Function: change_directory
		 *	Input:
		 	directoryname - name of subdirectory to navigate to
		 *	Return true if navigated to directory, else return false.

		 *	Change object to reflect a new directory. Map memory
		 	and update all fields to make that the working directory.
		 */
		bool change_directory(char *directory_name);	

		/*
		 *	Function: open_file
		 *	Input:
		 	name - filename in current directory, or absolute file path delimited by '/'
		 	options - flags
		 *	Return DirectoryListingObject set to named file.

		 *	Try to open file of given name by returning DirectoryListingObject
		 	to represent it. If argument is a single token, try to open as
		 	relative path. Else try absolute path from root. For file in root,
		 	~/filename works.
		 */
		DirectoryListObject *open_file(char *name, ...);

		/*
		 *	Function: create_file
		 *	Input:
		 	name - filename in current directory, or absolute file path delimited by '/'
		 	options - flags
		 *	Return true if file created, else false

		 *	Try to create file of given name. If argument is a single token, try to open as
		 	relative path. Else try absolute path from root. For file in root,
		 	~/filename works. Space allocation is handled. Function safe a application level.
		 */
		bool create_file(char *pathname, ...);

		/*
		 *	Function: create_directory
		 *	Input:
		 	name - name of new director
		 	options - flags

		 *	Create new subdirectory in current directory, with given pathname.
		 */
		void create_directory(char *pathname, ...);

		/*
		 *	Function: close_file
		 *
		 *	Close file currently represented by this object, and clean up object.
		 *	Unlock file, update info in directory listing, and update internal fields.
		 */
		void close_file();

		/*
		 *	Function: write_file
		 *	Input: input_string - string to insert in file at current position

		 *	Return number of characters written

		 *	Write given string to file. Allocate space as necessay.
		 	Return number of bytes written, which in normal cases should be
		 	length of input_string.
		 */
		int write_file(char *input_string);

		/*
		 *	Function: read_file
		 *	Input: num_characters - Maximum number of characters to read from file
		 *	Write given string to file. Allocate space as necessay.
		 *	Return number of bytes written, which in normal cases should be
		 	length of input_string.

		 *	Read from file, a maximum of n characters, copy into buffer, and
		 	return location of buffer.
		 *	Read not supported on string length more than 2^8 
		 */
		char *read_file(long n);

		/*
		 *	Function: delete_file
		 *	Input: filename - local name or absolute path of file to delete
		 *	Return true if file found and deleted, else false
		 
		 *	Search for file with name passed. If file found, show its blocks
			as available, and clear its directory listing.
		 	
		 */
		bool delete_file(char *filename);
		 
		/*
		 *	Function: seek_position
		 *	Input: N - byte number target for position pointer
		 *	Return number of bytes read, up to N.
		 */
		long seek_position(long N);

	private:
		// Buffer mapped to current data block. Deprec--Map whole memory in boot.
		// char open_memory[512];
		// Number of data block currently mapped to memory
		blockID data_block;
		// Pointer to physical location where current data block begins
		int current_block_offset;
		// Read/Write position in current block
		short current_position;
		// Pointer to parent starting block
		blockID parent_starting_block;
		// Bytes written since last update to DirectoryListing
		long bytes_written;
		// Still in directory? Set to false when simple file reached.
		bool in_directory;
		
		
		// Map to new block
		bool remap(blockID);

} ;

#endif