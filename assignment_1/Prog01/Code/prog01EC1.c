#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main (int argc, char *argv[])
{   
    if (argc < 3)
    {
        printf("Usage: \t%s <argument1> <argument2>\n", argv[0]);
        return 1;
    }
    else if (argc > 3) 
    {
        printf("Usage: \t%s <argument1> <argument2>\n", argv[0]);
        return 2;
    }
    int lastSlashIndex = strlen(argv[0])-1;
    while (argv[0][lastSlashIndex] != '/')
    {
        lastSlashIndex--;
    }
    printf("The executable %s was launched with two arguments:\n", argv[0] + lastSlashIndex + 1);
    return 0;
}