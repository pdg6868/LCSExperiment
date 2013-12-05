#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"

int** arr;

int lcs_memo( char *X, char *Y, int m, int n, char* ans)
{
    if (m == 0 || n == 0)
        return 0;
    if (arr[m][n]==0){
        if (X[m-1] == Y[n-1])
            arr[m][n] = 1 + lcs_memo(X, Y, m-1, n-1, ans);
        else
            arr[m][n] = MAX(lcs_memo(X, Y, m, n-1, ans), lcs_memo(X, Y, m-1, n, ans));
    }
    return arr[m][n];
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
    
    arr = (int**)lcs_malloc((x) * sizeof(int*));
    for(int i=0; i < x; i++) {
        arr[i] = (int *)lcs_malloc(sizeof(int)*(y));
        for(int j=0; j<y; j++){
            arr[i][j] = 0;
        }
    }
    
    //printf("%s\n%s\n", a,b);
    
    printf("Timing, Recursive with Memoization implementation:\n");
    double avg = timeit( lcs_memo, ittr, a, b, x-1, y-1, ansref );
    
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

