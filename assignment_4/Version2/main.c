#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// POSIX includes
#include <sys/types.h>
#include <dirent.h>
#include "program.h"

int main(int argc, char** argv)
{
	
	if (argc != 4) //need the name of program and a string parameter
	{
		printf("Usage: %s <number n of child processes> <directory path>\n", argv[0]);
		return 0;
	}

    int num_child_processes = atoi(argv[1]);
    char* file_name = argv[3];
   // printf("Number of Child Processes:%d\n\n", num_child_processes);

    // make arr of processes here
    Process* proc_arr = (Process*)malloc(sizeof(Process) * 3000);
    int proc_count = 0;

    // argv[2] is the dir path
    read_dir(argv[2], proc_arr, &proc_count, num_child_processes, file_name);
    //divide_work(argv[2]);
}