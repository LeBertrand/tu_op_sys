#include <stdio.h>
#include <unistd.h>
#include <math.h>

int main()
{
	printf("Page size is %zd\n", sysconf(_SC_PAGESIZE));
	printf("Page size is %zd\n", sysconf(_SC_PAGE_SIZE));

	printf("2^20 in hex is 0x%x\n", pow(2,20));

	return 0;
}