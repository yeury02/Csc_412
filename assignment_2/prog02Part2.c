#include <stdio.h>
#include <stdlib.h>

void fib(int fib1, int fib2, int every_triple);
int* fibo(int fib1, int fib2, int every_triplet, int sum);

int main (int argc, char* argv[])
{
    // declares array of size of arguments 
    int num_arr[(argc - 1)];
    int size = argc - 1;
    int i;
    // keeps track of every third num
    int count = 3;
    // keeps track of the sum of every third number
    // this is useful to declare an array later on
    int sum = 0;

    // stores all the arguments into an array
    for (i = 1; i < argc; i++)
    {
        if (i == count) {
            sum += atoi(argv[i]);
            count += 3;
        }
        num_arr[i - 1] = atoi(argv[i]);
    }
    printf("\n");

    // this variable is to keep count of every third number
    int triplets = 2;
    //int second_triplet = 5;
    int s;
    int k;
    for (i = 0; i < argc; i++)
    {
        if (i == triplets)
        {
            // if ( i == triplets && triplets >= 5)
            // {
            //     int fib1 = num_arr[i-2];
            //     int fib2 = num_arr[i-1];
            //     int every_triplet = num_arr[i];
            //     triplets += 3;
            //     int* new_fib = fibo(fib1, fib2, every_triplet, sum);
            //     printf("%d\n", new_fib[i]);

            // }
            int fib1 = num_arr[i-2];
            int fib2 = num_arr[i-1];
            int every_triplet = num_arr[i];
            // helper function that solves fibonacci
            //fib(fib1, fib2, every_triplet);
            // 
            //int* current = fibo(fib1, fib2, every_triplet, sum);
        }
        else if (i > 4)
        {
            int fib1 = num_arr[i-2];
            int fib2 = num_arr[i-1];
            int every_triplet = num_arr[i];
            // helper function that solves fibonacci
            //fib(fib1, fib2, every_triplet);
            int* new_fib = fibo(fib1, fib2, every_triplet, sum);
            for (i = 0; i<every_triplet; i++)
            {
                printf("%d ", new_fib[i]);
            }
        }
        
            // for (i = 0; i < every_triplet; i++)
            // {
            //     printf("%d ", current[i]);
            // }
        // if (i == second_triplet)
        // {
            
        //     //int* new_fib = (int*)malloc(sum * sizeof(int));
        //     for (k = i-2; k < second_triplet; k++)
        //     {
        //         int fib1 = num_arr[k];
        //         int fib2 = num_arr[k+1];
        //         int every_triplet = fib1 + fib2;
        //         int* new_fib = fibo(fib1, fib2, every_triplet, sum);

        //         // printf("%d ", *new_fib);
        //     }
        // }
        // for (i = )
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

int* fibo(int fib1, int fib2, int every_triplet, int sum)
{
    int tracker = 0;
    int* current = (int*)malloc(sum * sizeof(int*));
    int i;
    for (i = 0; i < every_triplet; i++)
    {
        if (tracker == 0)
        {
            current[i] = fib1;
            //printf("counter: %d, value:%d\n", tracker, current[i]);
            tracker++;
        }
        else if (tracker == 1)
        {
            current[i] = fib2;
            //printf("counter: %d, value:%d\n", tracker, current[i]);
            tracker++;
        }
        else
        {
            int curr = fib1 + fib2;
            fib1 = fib2;
            fib2 = curr;
            current[i] = curr;
            //printf("counter: %d, value:%d\n", tracker, current[i]);
        }
    }
    return current;
}