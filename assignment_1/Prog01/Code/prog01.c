#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[])
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
    else 
    {
        printf("The executable %s was launched with two arguments:\n", argv[0]);
        printf("\tThe first argument is: %s,\n", argv[1]);
        printf("\tThe second argument is: %s.\n", argv[2]);
        return 0;
    }
}