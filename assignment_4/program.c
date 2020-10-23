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

void read_dir(const char* dir_path, Process* proc_arr, int* proc_count, int num_child_processes)
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
                    // printf("%s\n", ep->d_name);
                    proc_arr[i].file_paths = new_path;
                    printf("%s", proc_arr[i].file_paths);
                    //read_file(new_path, &proc_arr[i]);
                    i++;
				}
                else
                {
                    free(new_path);
                }
                
			}
        }
    }
    proc_count[0] = i;
    char*** child_lists = (char***)malloc(sizeof(char**) * num_child_processes);
    int* child_list_sizes = (int*)malloc(sizeof(int) * num_child_processes);
    for (unsigned int i = 0; i < num_child_processes; i++)
    {
        child_lists[i] = (char**)malloc(sizeof(char*) * proc_count[0]);
        child_list_sizes[i] = 0;
        memset(child_lists[i], 0, sizeof(char*) * proc_count[0]);
    }

// Chris helped me do this!!!!!!!!!!
    // rename proc_count to num_files
    printf("file count: %d\n\n", *proc_count);

    // go thru all files, open them, figure out what the first number is, assign path to correct list
    for (unsigned int i = 0; i < proc_count[0]; i++)
    {
        FILE* pfile = fopen(proc_arr[i].file_paths, "r");
        if (pfile == NULL)
        {
            printf("Cannot open file: %s\n", proc_arr[i].file_paths);
        }
        else
        {
            int index = 0;
            fscanf(pfile, "%d ", &index);
            if (index < num_child_processes)
            {
                char** arr = child_lists[index];
                arr[child_list_sizes[index]] = proc_arr[i].file_paths;
                child_list_sizes[index] += 1;
            }
            else
            {
                printf("Process index %d exceeds number of proccesses\n", index);
            }
            fclose(pfile);
        }
    }

    //printing step --------------
    for (unsigned int i = 0; i < num_child_processes; i++)
    {
        printf("Child %u has %d file paths\n", i, child_list_sizes[i]);
        for (unsigned int j = 0; j < child_list_sizes[i]; j++)
        {
            printf("    Paths:%s\n", child_lists[i][j]);
        }
    }

    // sort content of each file and send sorted output to an output file
    Process** sub_data = (Process**)malloc(sizeof(Process*) * num_child_processes);

    for (unsigned int i = 0; i < num_child_processes; i++)
    {
        sub_data[i] = (Process*)malloc(sizeof(Process) * child_list_sizes[i]);
        for (unsigned int j = 0; j < child_list_sizes[i]; j++)
        {
            sub_data[i][j] = read_file(child_lists[i][j]);
        }
        qsort(sub_data[i], child_list_sizes[i], sizeof(Process), compare_to_sort);
        char* file1 = (char*)malloc(sizeof(char)*100);
        sprintf(file1, "output_%u.txt", i); // printing to string 
        //write_to_file(file1, sub_data[i], child_list_sizes[i]);
        free(file1);
    }
}

int compare_to_sort(const void* a, const void* b)
{
    const Process* fid_a = (const Process*)a;
    const Process* fid_b = (const Process*)b;
    int value = fid_a->line_num - fid_b->line_num;
    return value;
}

Process read_file(char* file_path)
{
    Process fork_ar;
    FILE *fp;
    fp = fopen(file_path, "r"); // read file
    if (fp == NULL)
    {
        printf("Cannot open file: %s\n", file_path);
    }
    fork_ar.line_data = (char*)malloc(sizeof(char) * 2000);
    int num_arr = fscanf(fp, "%d %d", &fork_ar.process_index, &fork_ar.process_index);
    fgets(fork_ar.line_data, 2000, fp);

    fclose(fp);
    return fork_ar;
}

void write_to_file(const char* file_path, const Process* fork_arr, int count);
{
    FILE *fptr1;

    fptr1 = fopen(file_path, "w");

    if (fptr != NULL)
    {
        for (unsigned int i = 0; i < count; i++)
        {
            fprintf(fprt1, "%s\n", fork_arr[i].line_data);
        }
        
    }
}



// void read_file(char* file_path, Process* struct_process)
// {
//     FILE* pfile = fopen(file_path, "r");
//     if (pfile == NULL)
//     {
//         printf("Cannot open file: %s\n", file_path);
//     }
    
//     fscanf(pfile, "%d %d", &struct_process->process_index, &struct_process->line_num);
//     struct_process->line_data = (char*)malloc(sizeof(char) * 2000);
//     // takes care of the last line, Chris helped me with this line, can get weird symbols if do not do this line
//     struct_process->line_data[0] = 0;
//     fgets(struct_process->line_data, 2000, pfile);
//     int len = strlen(struct_process->line_data);
//     if (len >= 1 && struct_process->line_data[len-1] == '\n')
//     {
//         struct_process->line_data[len-1] = '\0';
//     }
//     printf("%d %d %s\n", struct_process->process_index, struct_process->line_num, struct_process->line_data);

//     fclose(pfile);
// }

// void distributor(unsigned int k, const char* dir_path, int start_index, int end_index)
// {
//     struct dirent *ep;              // Pointer for directory entry   
//     DIR *dp = opendir(dir_path);     // opendir() returns a pointer of DIR type.
//     int i = 0;
		
//     char extension[100];       //file extensions will likely never be more than 100
//     while (ep = readdir(dp))
//     {   
//         //check if entry is a regular file
//         if (ep->d_type == DT_REG)
//         {
//             // char* new_path = (char*)malloc(sizeof(char) * (strlen(ep->d_name) + strlen(dir_path) + 2));
//             // strcpy(new_path, dir_path);
//             // strcat(new_path, "/");
//             // strcat(new_path, ep->d_name);

//             //check extension of file
//             get_extension(ep->d_name, extension);
            
//             //use the extension to determine what type of file it is
//             if (strcmp(extension, ".txt") == 0)
//             {
//                 for (unsigned int j = start_index; j <= end_index; j++)
//                 {
//                     printf("%u %s\n", j, ep->d_name);
//                 }
//             }
//         }
//     }
// }

// void divide_work(const char* dir_path)
// {
//     // 2 is the number of distributors

//     int files =  23;
//     int proc_num = 5;
//     int m = (files)/proc_num; 
//     int p = (files)%proc_num;
//     printf("m=%d p=%d\n", m, p);

//     int start_index = 0;
//     int end_index = m - 1;
//     // refers to the number of distrubutors for 
//     // if ((pp) % 5 == 0)
//     // {
//     for (unsigned int k = 0; k < proc_num; k++)
//     {
//         // the first p processes have one more file
//         if (k < p)
//         {
//             end_index++;
//         }
//         printf("%d %d\n", start_index, end_index);

//         // create process
//         distributor(k, dir_path, start_index, end_index);

//         //update range of indeces
//         start_index = end_index+1;
//         end_index = start_index + m - 1;
//     }
// }



// char* build_distributor_or_file_name(int index)
// {
//     "distr_" + index + ".txt";
// }