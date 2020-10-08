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

    read_dir(argv[1], images, patterns, &image_count, &pattern_count);
    read_dir(argv[2], images, patterns, &image_count, &pattern_count);
}

void read_dir(const char* dir_path, Image* images, Image* patterns, unsigned int* image_count, unsigned int* pattern_count)
{
    struct dirent *ep;              // Pointer for directory entry   
    DIR *dp = opendir(dir_path);     // opendir() returns a pointer of DIR type.

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
                char* new_path = (char*)malloc(sizeof(char) * (strlen(ep->d_name) + strlen(dir_path) + 2));
                strcpy(new_path, dir_path);
                strcat(new_path, "/");
                strcat(new_path, ep->d_name);
                printf("new path: %s", new_path);

				//only check the extension of files not directories
				get_extension(ep->d_name, extension);
				printf("    extension=%s\n", extension);
				
				//use the extension to determine what type of file it is
				if (strcmp(extension, ".img") == 0 && strcmp(extension, ".pat") == 0)
				{
                    Image tmp = read_image(new_path);
                    if (tmp.numRows > 0 && tmp.numCols > 0)
                    {
                        if (strcmp(extension, ".img") == 0)
                        {
                            images[*image_count] = tmp;
                            *image_count++;
                        }
                        else if (strcmp(extension, ".pat") == 0)
                        {
                            patterns[*pattern_count] = tmp;
                            *pattern_count++;
                        }
                    }
                    else 
                    {
                        printf("Could not load image file %s\n", new_path);
                    }
				}
			}
        }
    }
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
    printf("Made it here\n");
    fflush(stdout);
    // read dimensions
    fseek(img_file, 0, SEEK_SET);
    fscanf(img_file, "%u %u\n", &my_image.numCols, &my_image.numRows);
    printf("%u %u\n", my_image.numCols, my_image.numRows);

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
            printf("%c", my_image.data[i][j]);
        }
        printf("\n");

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