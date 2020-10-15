#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// POSIX includes
#include <sys/types.h>
#include <dirent.h>
#include "program.h"

//get the extension and write to extensio string
void get_extension(const char* search_string, char* extension)
{
	char* dot = strchr(search_string, '.');
	if (dot == NULL) //no file extension case
	{
		extension[0] = '\0'; //this will make extension a blank string
	}
	else
	{
		char* tmp = dot;
		while (tmp != NULL)
		{
			tmp = strchr(tmp + 1, '.');
			if (tmp != NULL)
				dot = tmp;
		}
		strcpy(extension, dot);
	}
}

void read_dir(const char* dir_path)
{
	struct dirent *ep;              // Pointer for directory entry   
    DIR *dp = opendir(dir_path);     // opendir() returns a pointer of DIR type.

	printf("Directory=%s\n", dir_path);
	//  //get char* outputPath as argument
	// FILE* output_path;

    if (dp != NULL)
	{
		char extension[100];       //file extensions will likely never be more than 100
        while (ep = readdir(dp))
        {   
            //check if this entry is a directory
			if (ep->d_type == DT_DIR)
			{
                continue;
			}
            //check if entry is a regular file
            else if (ep->d_type == DT_REG)
			{
                char* new_path = (char*)malloc(sizeof(char) * (strlen(ep->d_name) + strlen(dir_path) + 2));
                strcpy(new_path, dir_path);
                strcat(new_path, "/");
                strcat(new_path, ep->d_name);

				//check extension of file
				get_extension(ep->d_name, extension);
				
				//use the extension to determine what type of file it is
				if (strcmp(extension, ".txt") == 0)
				{
                    printf("%s\n", ep->d_name);
				}
			}
        }
    }
}