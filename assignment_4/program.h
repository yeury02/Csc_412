#ifndef PROGRAM_H
#define PROGRAM_H

typedef struct Process {
    int process_index, line_num, process_ID; // process_ID will not be used for version 1
    char* line_data; // not needed for version 1
} Process;

void get_extension(const char* search_string, char* extension);
void read_dir(const char* dir_path, Process* proc_arr, int* proc_count);
void read_file(char* file_path, Process* struct_process);

#endif