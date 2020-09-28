#include <stdio.h>
#include <stdlib.h>

void fib(char* exec_name, int fib1, int fib2, int n);

int main(int argc, char* argv[]) 
{
    // get command line arguments and turn it to the appropiate date type
    char* exec_name = argv[0];
    int fib1 = atoi(argv[1]);
    int fib2 = atoi(argv[2]);
    int n = atoi(argv[3]);
    // calls fib func
    fib(exec_name, fib1, fib2, n);
    
    return 0;
}

void fib(char* exec_name, int fib1, int fib2, int n)
{
    // makes sure all the numbers are positive
    if (fib1 > 0 && fib2 > 0 && n > 0) 
    {
        // check second arg is bigger than the first
        if (fib2 > fib1) 
        {
            // starting point of fib sequence
            int i = 2;
            // 6 terms of the Fibonacci series with F1=2 and F2=5:
            printf("\n");
            printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n", n, fib1, fib2);
            printf("F1=%d,\tF2=%d,\t", fib1, fib2);
            for (i = 2; i < n; i++)
            {
                int curr = fib1 + fib2;
                fib1 = fib2;
                fib2 = curr;
                // as long as I am not in the last fib subsequent
                if (i + 1 != n)
                {
                    printf("F%d=%d,\t", i+1, curr);
                }
                // do this if I am in the last fib subsequent
                // Did this because of the extra , at the end
                else 
                {
                    printf("F%d=%d\n", i+1, curr);
                    printf("\n");
                }
            }
        }
        else
        {   
            printf("\n");
            printf("error: The second argument must be strictly larger than the first\n");
            printf("\n");
        }
    } 
    else
    {
        // one or more of the argument\s do not follow the rules
        printf("\n");
        printf("Usage:\t%s  F1 F2 n, with F2>F1>0 and n>0.\n", exec_name);
        printf("\n");
    }
}