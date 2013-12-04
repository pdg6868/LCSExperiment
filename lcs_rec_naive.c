//
//  lcs.c
//  Multiple Implemetations of the LCS algorithm
//
//  Authors: Pat Gillis and Alex Dean
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"


//Basic Recursive as Implied by 15.1 on pg. 392
int lcs_naive(char* A, char* B, char* ans)
{
	if (*A == '\0' || *B == '\0')
        return 0;
	else if (*A == *B)
        return 1 + lcs_naive(A+1, B+1, ans);
	else
        return MAX(lcs_naive(A+1,B, ans), lcs_naive(A,B+1, ans));
}

//This main assumes that str1 is in argv[1]
//and str2 is in argv[2]
// This is ensured by the main script
int main(int argc, char** argv){

    int ittr,x,y;
    sscanf (argv[1], "%i", &ittr);
    scanf("%d %d", &x, &y);

    char* a = (char*)malloc(x * sizeof(char)+1);
    char* b = (char*)malloc(y * sizeof(char)+1);
    char* ansref = (char*)malloc( x * sizeof(char) );
    
    scanf("%s %s", a, b);
    
    printf("Timing, Nieve Recursive implementation:\n");
    timeit( lcs_naive, ittr, a, b, ansref);
    
    free( ansref );

    return 0;
}
