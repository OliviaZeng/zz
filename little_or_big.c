#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
	int i = 0x1234;

	printf("%x\n", (*(char*)&i));
}
