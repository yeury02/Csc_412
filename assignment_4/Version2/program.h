/*
 * Author: Yeury Galva Liriano
 * Date: November 25, 2020
 * Description: Header file for creating child processes
*/

#ifndef PROGRAM_H
#define PROGRAM_H

// rename struct to be for sorting data
typedef struct Process {
    int process_index, line_num, process_ID; // process_ID will not be used for version 1
    char* line_data;
    char* file_paths;
} Process;

/**
 * @author Yeury Galva Liriano
 * @date October 25, 2020
 * @brief get_extension function gets the extension of a file
 * @details This function is useful to know what kinds of files are being looked at, by looking at the extension. Function is called in the read_dir function
 * @param search_string The pointer to search for a specific extension
 * @param extension This is an pointer to an array of characters to basically hold the extension of file
 */
void get_extension(const char* search_string, char* extension);

/**
 * @author Yeury Galva Liriano
 * @date October 25, 2020
 * @brief read_dir function reads through a folder path
 * @details This function does alot more than it is meant to do. Shoud of proven down this function into multiple functions. Function finds all the files from a folder path. Assigns each file to a distributors and sorts the files by line number.
 * @param dir_path The pointer to a directory path
 * @param proc_arr Instance of my struct, used to assign all the new created file paths to my data structure
 * @param proc_count Used to count how many files exist in the dir_path folder
 * @param num_child_processes Used to count how many different distributors are in dir_path
 */
void read_dir(const char* dir_path, Process* proc_arr, int* proc_count, int num_child_processes, char* file_name);

/**
 * @author Yeury Galva Liriano
 * @date October 25, 2020
 * @brief compare_to_sort function compares files by their line number
 * @details This function is useful and necessary to have because it compares two files and returns which one has the highest line number. This is then used on the qsort.
 * @param a A pointer to a specific file
 * @param b A pointer to a specific file
 */
int compare_to_sort(const void* a, const void* b);

/**
 * @author Yeury Galva Liriano
 * @date October 25, 2020
 * @brief read_file function reads a file 
 * @details This function is useful and necessary to have because it reads each file and stores it's data in my struct
 * @param file_path A pointer to a specific file
 * @return Function returns an instance of my struct with the corresponding data grabbed when reading the file
 */
Process read_file(char* file_path);

#endif