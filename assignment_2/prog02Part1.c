#include <stdio.h>
#include <stdlib.h>

void fib(int fib1, int fib2, int n)
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
            printf("F%d=%d", i+1, curr);
        }
    }
    printf("\n");
}

int main(int argc, char* argv[]) 
{
    // get command line arguments and turn it into integers
    int fib1 = atoi(argv[1]);
    int fib2 = atoi(argv[2]);
    int n = atoi(argv[3]);
    fib(fib1, fib2, n);
    
    return 0;
}