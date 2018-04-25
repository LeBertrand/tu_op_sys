#include <stdlib.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
	
	FILE *drive = fopen(argv[1], "w");
	int index = 0x200000;
	while(index--){
		fputc(0, drive);
	}

	return EXIT_SUCCESS;
}