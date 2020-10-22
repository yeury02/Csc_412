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
    printf("file count: %d\n\n", *proc_count);
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
    // 2 is the number of distributors

    int files =  23;
    int proc_num = 5;
    int m = (files)/proc_num; 
    int p = (files)%proc_num;
    printf("m=%d p=%d\n", m, p);

    int start_index = 0;
    int end_index = m - 1;
    // refers to the number of distrubutors for 
    // if ((pp) % 5 == 0)
    // {
    for (unsigned int k = 0; k < proc_num; k++)
    {
        // the first p processes have one more file
        if (k < p)
        {
            end_index++;
        }
        printf("%d %d\n", start_index, end_index);

        // create process
        //distributor(k, folderpath, start_index, end_index);

        // update range of indeces
        start_index = end_index+1;
        end_index = start_index + m - 1;
    }
}