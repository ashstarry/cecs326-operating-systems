/*
* PROGRAM NAME: lab1.c
* 
* DESCRIPTION: Write a C program named lab1 to store N random numbers in an array named randA[ ] and determine the 
* largest, smallest, and average values of this array. Display the array elements that store these values. The 
* random numbers range from -100 to +100. N is a command line argument. Use the debugger to go 
* through the program. Use all the debugger commands. 
* 
* AUTHOR: kristin peterson
*/
# include <time.h>
# include <stdio.h>
# include <stdlib.h>


void validate_args(int argc, char *argv[]);
double calc_average(int sum, int elements);

int main(int argc, char *argv[])
{
    /* Variable declarations / instantiations */
    int n,i,j,min,max,sum;
    int *randA;
    min = 0;
    max = 0;
    sum = 0;
    validate_args(argc, argv);

    /* Convert command line argument to integer and assign to n */
    n = atoi(argv[1]);

    /* Dynamically instantiate array of size n */
    randA = (int *) malloc(sizeof(int) * n);

    /* Initialize Random Number Generator (using time as seed) */
    srand((int) time(NULL));

    printf("\nPopulating the array with %d random number(s) ranging from -100 to +100.\n", n);
    printf("\nindex\t element");

    /*
    * Populate the array with N random numbers ranging from -100 to +100
    * While populating the array find the minimum and maximum values and the 
    * sum of all values in the array
    */
    for(i = 0; i < n; i++) {
        j = rand() % 201 - 100;
        randA[i] = j;
        printf("\n%d\t %d", i, j);
        sum += j;
        if(j > max)
            max = j;
        if(j < min)
            min = j;
    }

    /* Print results */
    printf("\n\nMax: %d", max);
    printf("\nMin: %d", min);
    printf("\nAvg: %f\n", calc_average(sum, n));

    /* Free memory assigned to randA array */
    free(randA);
}

/* 
* Function: calc_average
* ----------------------
* Calculates the average of a given sum/elements pair
* 
* sum: the sum of elements
* elements: the number of elements that comprise the sum
*
* returns: average value of a sum comprised of elements
*/
double calc_average(int sum, int elements)
{
    return (double)sum / elements;
}

/*
* Function: valdiate_args
* -----------------------
* Validates the command line arguments, exits program with warning if 
* arguments are invalid.
*
* argc: number of command line arguments entered
* argv: array containing command line arguments
*/ 
void validate_args(int argc, char *argv[])
{
    /* Confirm that only one command line argument is given */
    if(argc != 2) {
        printf("Wrong number of command line arguments. Please provide one argument.\n");
        exit(1);
    }

    int n = atoi(argv[1]);

    /* Exit program if input is <= 0 (ie. non-integer input, zero, or negative values) */
    if(n <= 0) {
        printf("Erroneous input, please enter an integer greater than 0 as command line argument.\n");
        exit(1);
    }
}