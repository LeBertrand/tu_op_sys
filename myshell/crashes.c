#include <stdio.h>
#include <stdlib.h>

int main()
{
	int f = 8;
	f/=0;
	printf("Divide by zero yields %d.\n", f);

	return 0;
}
