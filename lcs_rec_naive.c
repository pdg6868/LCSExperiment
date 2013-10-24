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
int lcs_naive(char* A, char* B)
{
	if (*A == '\0' || *B == '\0')
        return 0;
	else if (*A == *B)
        return 1 + lcs_naive(A+1, B+1);
	else
        return MAX(lcs_naive(A+1,B), lcs_naive(A,B+1));
}

//This main assumes that str1 is in argv[1]
//and str2 is in argv[2]
// This is ensured by the main script
int main(int argc, char** argv){
    
    int ittr;
    sscanf (argv[1], "%i", &ittr);
    
    printf("Timing, Nieve Recursive implementation:\n");
    double ret = timeit( lcs_naive, ittr, argv[2],argv[3]);
    printf("%f\n", ret);
    
    return 0;
}