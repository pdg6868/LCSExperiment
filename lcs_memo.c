#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"

int** arr;
int m;
int n;

int lcs_memo( char *A, char *B, char* ans)
{
    /*
    if (*A == '\0' || *B == '\0')
        return 0;
    if (arr[m][n]==0){
        if (*A == *B)
            return 1 + lcs_memo(A+1, B+1, ans);
        else
            return MAX(lcs_memo(A+1,B, ans), lcs_memo(A,B+1, ans));
    }
    return arr[m][n];
     */
    return 0;
}

int main(int argc, char** argv){
    int ittr, x, y;
    extern int memusage;
    memusage = 0;
    sscanf (argv[1], "%i", &ittr);
    
    scanf("%d %d", &x, &y);
    char* a = (char*)malloc(x * sizeof(char)+1);
    char* b = (char*)malloc(y * sizeof(char)+1);
    char* ansref = (char*)malloc( x * sizeof(char) );
    
    scanf("%s %s", a, b);
    
    m=x;n=y;
    arr = (int**)lcs_malloc((x) * sizeof(int*));
    for(int i=0; i < x; i++) {
        arr[i] = (int *)lcs_malloc(sizeof(int)*(y));
    }
    
    //printf("%s\n%s\n", a,b);
    
    printf("Timing, Recursive with Memoization implementation:\n");
    double avg = timeit( lcs_memo, ittr, a, b, ansref );
    
    printf("Dynamic Memory Allocated: %d bytes\n", memusage/ittr);
    // Time complexity is Theta m*n
    // So time const is avgTime/(m*n)
    printf("Time Const: %e\n", avg/(x*y));
    
    
    free( ansref );
    for(int i=0; i < x; i++) {
        free(arr[i]);
    }
    free(arr);
    
    return 0;

}

