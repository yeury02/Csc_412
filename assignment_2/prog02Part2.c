#include <stdio.h>
#include <stdlib.h>

void fib(int fib1, int fib2, int every_triplet);
// void appear_once(int curr_num, )

int main (int argc, char* argv[])
{
    // declares array of size of arguments 
    int num_arr[(argc - 1)];
    int i;

    // stores all the arguments into an array
    for (i = 1; i < argc; i++)
    {
        num_arr[i - 1] = atoi(argv[i]);
    }
    printf("\n");

    // this available is to keep count of every third number
    int triplets = 2;
    for (i = 0; i < argc; i++)
    {
        if (i == triplets)
        {
            int fib1 = num_arr[i-2];
            int fib2 = num_arr[i-1];
            int every_triplet = num_arr[i];
            triplets += 3;
            // helper function that solves fibonacci
            fib(fib1, fib2, every_triplet);
        }
    }
    printf("\n");
}

void fib(int fib1, int fib2, int every_triplet)
{
    // starting point of fib sequence
    int i = 2;
    printf("%d terms of the Fibonacci series with F1=%d and F2=%d:\n", every_triplet, fib1, fib2);
    //prints the first two of the fib sequence
    printf("F1=%d,\tF2=%d,\t", fib1, fib2);
    for (i = 2; i < every_triplet; i++)
    {
        int curr = fib1 + fib2;
        fib1 = fib2;
        fib2 = curr;
        // as long as I am not in the last fib subsequent
        if (i + 1 != every_triplet)
        {
            printf("F%d=%d,\t", i+1, curr);
        }
        // do this if I am in the last fib subsequent
        // Did this because of the extra , at the end
        else
        {
            printf("F%d=%d\n", i+1, curr);
        }
    }
}   