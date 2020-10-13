#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>   // for PATH_MAX
// POSIX includes
#include <sys/types.h>
#include <dirent.h>

typedef struct Image {
    unsigned int numCols, numRows;
    char** data;
} Image;

void get_extension(const char* search_string, char* extension);
Image read_image(char* path);
void read_dir(const char* dir_path, Image* images, Image* patterns, unsigned int* image_count, unsigned int* pattern_count);
void print_matches(Image* image, Image* pattern);
char* get_file_name(char* pattern_file_path);
void open_file(char* output_path);
int intLength(int n);

int main(int argc, char* argv[])
{
    // array of images
    Image* images = NULL;
    // array of patterns
    Image* patterns = NULL;

    unsigned int image_count = 0;
    unsigned int pattern_count = 0;
    // if there are not 4 arguments, can't compute anything
    if (argc != 4)
    {
        printf("Usage: %s <directory path>\n", argv[0]);
    }
    images = (Image*)malloc(sizeof(Image)*200);
    patterns = (Image*)malloc(sizeof(Image)*200);

    patterns[0] = read_image(argv[1]);
    read_dir(argv[2], images, patterns, &image_count, &pattern_count);

    char* output_dir_path = argv[3];
    char* new_file_name = get_file_name(argv[1]);
    char* output_path = (char*)calloc(strlen(output_dir_path) + strlen(new_file_name) + 2, sizeof(char));
    strcpy(output_path, output_dir_path);
    // if outputDir_path doesn't end with /
        strcat(output_path, "/");
    strcat(output_path, new_file_name);
    // strcat(output_path, new_file_name);
    //printf("%s\n", output_path);
    //void open_file(char* output_path);
    free(output_path);
}

//get the extension and write to extension string
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
			//printf("tmp=%s\n", tmp);
			if (tmp != NULL)
				dot = tmp;
		}
		//printf("dot=%s\n", dot);
		strcpy(extension, dot);
	}
}

Image read_image(char* path)
{   
    Image my_image;
    // creates file pointer and points to the file that opens
    FILE* img_file = fopen(path, "r");
    if (img_file == NULL)
    {
        printf("Cannot open file %s\n", path);
        my_image.numCols = 0;
        my_image.numRows = 0;
        return my_image;
    }
    fseek(img_file, 0, SEEK_END); // go to end of file

    if (ftell(img_file) == 0)     // check if file is empty
    {
        printf("File Empty\n");
        my_image.numCols = 0;
        my_image.numRows = 0;
        return my_image;
    }
    //printf("Made it here\n");
    //fflush(stdout);
    // read dimensions
    fseek(img_file, 0, SEEK_SET);
    fscanf(img_file, "%u %u\n", &my_image.numCols, &my_image.numRows);
    //printf("%u %u\n", my_image.numCols, my_image.numRows);

    my_image.data = (char**)calloc(my_image.numRows, sizeof(char*));
    for (unsigned int i=0; i<my_image.numRows; i++)
    {
        // allocate row i
        my_image.data[i] = calloc(my_image.numCols, sizeof(char));
        // read the row
        for (unsigned int j=0; j<my_image.numCols; j++)
        {
            //fscanf(img_file, "%c", my_image.data[i]+j);
            my_image.data[i][j] = (char)getc(img_file);
            // printf("%c", my_image.data[i][j]);
        }
        // read the end of linear char, V1
        char eol = (char)getc(img_file);
        if (eol == '\r')
        {
            (char)getc(img_file);
        }
        //fscanf(img_file, "%c", &eol);
    }
    fclose(img_file);
    return my_image;
}

void read_dir(const char* dir_path, Image* images, Image* patterns, unsigned int* image_count, unsigned int* pattern_count)
{
    struct dirent *ep;              // Pointer for directory entry   
    DIR *dp = opendir(dir_path);     // opendir() returns a pointer of DIR type.

    //get char* outputPath as argument
	FILE* output_path;

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

				//only check the extension of files not directories
				get_extension(ep->d_name, extension);
				
				//use the extension to determine what type of file it is
				if (strcmp(extension, ".img") == 0)
				{
                    Image tmp = read_image(new_path);

                    printf("%s\n", ep->d_name);
                    // call print matches here
                    print_matches(&tmp, patterns);
				}
			}
        }
    }
}

// this function prints the matches found in img and pat
// could have a different return type if I finished assignment
void print_matches(Image* image, Image* pattern)
{
    // array 
    char* matches = (char*)calloc(1, sizeof(char));
    int count = 0;
    // iterate overy every possile positio of patter in the image
    for (unsigned int i=0; i<image->numRows-pattern->numRows; i++)
    {
        for (unsigned int j=0; j<image->numCols-pattern->numCols; j++)
        {
            // do we find the pattern at locationn (i,j) in the loop
            int match = 1;
            // loop over the entire pattern until reach the end
            for (unsigned int k=0; k<pattern->numRows; k++)
            {
                for (unsigned int l=0; l<pattern->numCols; l++)
                {
                    // compare
                    if (pattern->data[k][l] == image->data[i+k][j+l])
                    {
                        continue;
                    }
                    else
                    {
                        match = 0;
                    }
                }
            }
            if (match) 
            {
                count++;
                int int_space = intLength(i);
                int int_space2 = intLength(j);

                char* tmp = (char*)calloc(strlen(matches)+int_space+int_space2+count+2, sizeof(char));
                sprintf(tmp, "%s %d %d",matches, i,j);
                matches = tmp;
            }
        }
    }
    if (count == 1)
    {
        printf("   %d%s\n", count, matches);
    }
    else if ( count > 1)
    {
        // using a tab was too much
        printf("   %d%s\n", count, matches);
    }
    else {
        printf("   0\n");
    }
}

// /pattern1.pat
// this function is to get the name for the exact output file
char* get_file_name(char* pattern_file_path)
{
    // _Matches.txt
    char* add_to_file_name = "_Matches.txt";
    char* file_path = strrchr(pattern_file_path, '/');
    // allocate memory for file name
    char* file_name = (char*)calloc((strlen(file_path)-5) + strlen(add_to_file_name), sizeof(char));
    for (unsigned int i=0; i<strlen(file_path); i++)
    {
        if (file_path[i+1] == '.')
        {
            break;
        }
        file_name[i] = file_path[i+1];
    }
    strcat(file_name, add_to_file_name);
    return file_name;
}

// did not get the chance to output to an output file
// void open_file(char* output_path)
// {
//     FILE *out_file = fopen(output_path, "w"); 
//     fprintf(out_file, "%s %s %s %d", "We", "are", "in", 2012);
//     if ( out_file == NULL ) 
//     { 
//         printf( "file failed to open." ); 
//     } 
//     else
//     {
//         printf("file is now opened");
//     }
// }

// function to know the length of an integer, to know the space to allocae dynamically
int intLength(int n) {
	if (n<10)
    {
	    return 1;
    }
	else if (n<100)
    {
	    return 2;
    }
    else if (n<1000)
    {
        return 3;
    }
    else if (n<10000)
    {
        return 4;
    }
    else
    {
        return 5;
    }
}

