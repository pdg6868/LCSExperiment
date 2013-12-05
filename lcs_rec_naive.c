//
//  lcs_rec_naive.c
//
//  Authors: Pat Gillis and Alex Dean
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "lcs.h"
#include "timeit.h"


//Basic Recursive as Implied by 15.1 on pg. 392
int lcs_naive(char* X, char* Y, int m, int n, char** ans)
{
	if (m == 0 || n == 0)
        return 0;
    if(X[m-1] == Y[n-1])
        return 1 + lcs_naive(X, Y, m-1, n-1, ans);
    else
        return MAX(lcs_naive(X, Y, m, n-1, ans), lcs_naive(X, Y, m-1, n, ans));
}

//This main assumes that str1 is in argv[1]
//and str2 is in argv[2]
// This is ensured by the main script
int main(int argc, char** argv){

    int ittr,x,y;
    sscanf(argv[1], "%i", &ittr);
    scanf("%d %d", &x, &y);

    char* a = (char*)malloc(x * sizeof(char)+1);
    char* b = (char*)malloc(y * sizeof(char)+1);
    char* ansref = (char*)malloc( x * sizeof(char) );
    
    scanf("%s %s", a, b);
    
    printf("Timing, Nieve Recursive implementation:\n");
    double avg = timeit( lcs_naive, ittr, a, b, x, y, &ansref);
    extern int memusage;
    printf("Dynamic Memory Allocated: %d bytes\n", memusage/ittr);
    // Time complexity is O(2^{m*n})
    // So time const is avgTime/(2^{m*n})
    printf("Time Const: %e\n", avg/(pow(2,x*y)));

    free( a ); free( b );
    free( ansref );

    return 0;
}
