#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"

int lcs_with_memo( char *X, char *Y, int m, int n, int** arr ) 
{
    // If Either string is NULL, no recursions have been performed.
    if (m == 0 || n == 0) return 0;

    // If we have not computed the lookup for this position, we must recurse.
    if (arr[m][n]==0){
        if (X[m-1] == Y[n-1])
            arr[m][n] = 1 + lcs_with_memo(X, Y, m-1, n-1, arr);
        else
            arr[m][n] = MAX( lcs_with_memo(X, Y, m, n-1, arr), 
                             lcs_with_memo(X, Y, m-1, n, arr));
    }

    // Will return 0 if we haven't recursed or N for the recurse count.
    return arr[m][n];
}

int lcs_memo( char *X, char *Y, int m, int n, char** ans)
{
    // Build our lookup table.
    int** arr = (int**)lcs_malloc((m+1) * sizeof(int*));
    for(int i=0; i <= m; i++) {
        arr[i] = (int *)lcs_malloc((n+1) * sizeof(int));
        for(int j=0; j <= n; j++){
            arr[i][j] = 0;
        }
    }

    // Actually run the memoized function.
    int res = lcs_with_memo( X, Y, m, n, arr );

    // Clean up time.
    for(int i=0; i < m; i++) { free(arr[i]); }
    free(arr);
   
    // Return the memoized result. 
    return res;
}

int main(int argc, char** argv){
    int ittr, x, y;
    extern ulong memusage;
    memusage = 0L;
    sscanf (argv[1], "%i", &ittr);
    
    scanf("%d %d", &x, &y);
    char* a = (char*)malloc( (x+1) * sizeof(char));
    char* b = (char*)malloc( (y+1) * sizeof(char));
    char* ansref = (char*)malloc( (x+1) * sizeof(char) );
    
    scanf("%s %s", a, b);
    
    //printf("%s\n%s\n", a,b);
    
    printf("Timing, Recursive with Memoization implementation:\n");
    double avg = timeit( lcs_memo, ittr, a, b, x, y, &ansref );
    
    printf("Dynamic Memory Allocated: %lu bytes\n", memusage/ittr);
    // Time complexity is Theta m*n
    // So time const is avgTime/(m*n)
    printf("Time Const: %e\n", avg/(x*y));
 
    free( a ); free( b );
    free( ansref );
    
    return 0;
}

