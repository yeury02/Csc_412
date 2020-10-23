#ifndef PROGRAM_H
#define PROGRAM_H

// rename struct to be for sorting data
typedef struct Process {
    int process_index, line_num, process_ID; // process_ID will not be used for version 1
    char* line_data;
    char* file_paths;
} Process;

typedef struct Task {
    int num_files;   // 4
    int* file_index; // array indeces {0, 5, 8, 12}
}Task;

void get_extension(const char* search_string, char* extension);
void read_dir(const char* dir_path, Process* proc_arr, int* proc_count, int num_child_processes);
int compare_to_sort(const void* a, const void* b);
Process read_file(char* file_path);
void write_to_file(const char* file_path, const Process* fork_arr, int count);
// void read_file(char* file_path, Process* struct_process);
// void distributor(unsigned int k, const char* dir_path, int start_index, int end_index);
// void divide_work(const char* dir_path);


//char* build_distributor_or_file_name(int index);

// distrubutor_or_func(<arguments>) writes results to file

#endif