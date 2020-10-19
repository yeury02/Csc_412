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

void read_dir(const char* dir_path, Process* proc_arr, int* proc_count)
{
	struct dirent *ep;              // Pointer for directory entry   
    DIR *dp = opendir(dir_path);     // opendir() returns a pointer of DIR type.
    int i = 0;

	printf("Directory=%s\n\n", dir_path);
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
                    printf("%s --> ", ep->d_name);
                    read_file(new_path, &proc_arr[i]);
                    i++;
				}
			}
        }
    }
    proc_count[0] = i;
    printf("%d\n", *proc_count);
}

void read_file(char* file_path, Process* struct_process)
{
    FILE* pfile = fopen(file_path, "r");
    if (pfile == NULL)
    {
        printf("Cannot open file: %s\n", file_path);
    }
    // fseek(pfile, 0, SEEK_END); // go to end of file

    // if (ftell(pfile) == 0)     // check if file is empty
    // {
    //     printf("File Empty\n");
    //     exit(1);
    // }

    fscanf(pfile, "%d %d", &struct_process->process_index, &struct_process->line_num);
    struct_process->line_data = (char*)malloc(sizeof(char) * 2000);
    // takes care of the last line, Chris helped me with this line, can get weird symbols if do not do this line
    struct_process->line_data[0] = 0;
    fgets(struct_process->line_data, 2000, pfile);
    printf("%d %d %s\n", struct_process->process_index, struct_process->line_num, struct_process->line_data);

    fclose(pfile);
}

void divide_work(int* proc_count)
{
    int i;
    int count = 1;
    // refers to the number of distrubutors for 
    if ((*proc_count + 5) % 2 == 0)
    {
        for (i=0; i<2; i++)
        {
            // allocates space for each distibutor to then be used to look at files of range
            char* distributor = (char*)malloc(sizeof(char) * (*proc_count/2));
            //void read_file(char* file_path, Process* struct_process);
            printf("%d\n", i);
        }
    }
    else
    // do best to split the work as even as possible 
    {
        int zp = 2 - ((*proc_count+5) % 2); 
        int pp = (*proc_count+5)/2; 
        for(int i=0;i<2;i++)  
        { 
            if(i >= zp)
            {
                pp++;
                printf("%d\n", pp++);

            }
            else
            {
                printf("%d\n", pp);
            }
        }
    }
}