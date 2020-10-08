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
    //char locationName[100]; //name of the location
    //int imageNumber; //number of entry (image xxx)
} Image;

void get_extension(const char* search_string, char* extension);
Image read_image(char* path);

int main(int argc, char* argv[])
{
    // if there are not 4 arguments, can't compute anything
    if (argc != 4)
    {
        printf("Usage: %s <directory path>\n", argv[0]);
    }

    struct dirent *ep;              // Pointer for directory entry   
    DIR *dp = opendir(argv[1]);     // opendir() returns a pointer of DIR type.
    //printf("Directory=%s\n", argv[1]);

    char path[PATH_MAX + 1]; 

    if (dp != NULL)
	{
		char extension[100];       //file extensions will likely never be more than 100
        while (ep = readdir(dp))
        {   
            printf("entry=%s\n", ep->d_name);

            //check if this entry is a directory
			if (ep->d_type == DT_DIR)
			{
				printf("    Entry is a directory\n");
			}
            //check if entry is a regular file
            else if (ep->d_type == DT_REG)
			{
				//only check the extension of files not directories
				get_extension(ep->d_name, extension);
				printf("    extension=%s\n", extension);
				
				//use the extension to determine what type of file it is
				if (strcmp(extension, ".img") == 0)
				{
					printf("    File is an image file\n");
                    realpath(ep->d_name, path);
                    printf ("%s\n", path);
                    read_image(path);
				}
			}
        }
    }
    return 0;
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
    // creates file pointer and points to the file that opens
    FILE* img_file = fopen(path, "r");
    Image my_image;

    fseek(img_file, 0, SEEK_END); // go to end of file

    if (ftell(img_file) == 0)     // check if file is empty
    {
        printf("File Empty\n");
        exit(0);
    }
    printf("Made it here\n");
    fflush(stdout);
    // read dimensions
    fscanf(img_file, "%u%u\n", &my_image.numCols, &my_image.numRows);

    my_image.data = (char**)calloc(my_image.numRows, sizeof(char*));
    for (unsigned int i=0; i<my_image.numRows; i++)
    {
        // allocate row i
        my_image.data[i] = calloc(my_image.numCols, sizeof(char));
        // read the row
        for (unsigned int j=0; j<my_image.numCols; j++)
        {
            fscanf(img_file, "%c", my_image.data[i]+j);
        }
        // read the end of linear char, V1
        char eol;
        fscanf(img_file, "%c", &eol);
    }
    // if file is not empty,
    // allocate memory for numrows and numcols

    fclose(img_file);
    
    return my_image;
}