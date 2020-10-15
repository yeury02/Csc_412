#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// POSIX includes
#include <sys/types.h>
#include <dirent.h>
#include "program.h"

int main(int argc, char** argv)
{
	
	if (argc != 3) //need the name of program and a string parameter
	{
		printf("Usage: %s <number n of child processes> <directory path>\n", argv[0]);
		return 0;
	}

    read_dir(argv[2]);
}