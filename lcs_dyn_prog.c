//
//  lcs_dyn_prog.c
//  Multiple Implemetations of the LCS algorithm
//
//  Authors: Pat Gillis and Alex Dean
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"

//Prints the LCS from table generated
//by lcs algorithm
void print_lcs(int** b, char* X, int i, int j){
    if (i==0 || j==0)
        return;
    if (b[i][j] == DIAG){
        print_lcs(b,X,i-1,j-1);
        //printf("%c",X[i-1]);
    }
    else if(b[i][j] == UP){
        print_lcs(b,X,i-1,j);
    }
    else{
        print_lcs(b,X,i,j-1);
    }
}

//LCS algorithm as on pg. 394 of textbook
int lcs_dyn_prog(char* X, char* Y, char* ans){
    //To move outside function and pass in?
    int m = strlen(X);
    int n = strlen(Y);

    // Create 'C' and 'B' matrices
    int** c = (int**)lcs_malloc((m+1) * sizeof(int*));
    for(int i=0; i <= m; i++) {
        c[i] = (int *)lcs_malloc(sizeof(int)*(n+1));
    }
    int** b = (int**)lcs_malloc((m+1) * sizeof(int*));
    for(int i=0; i <= m; i++) {
        b[i] = (int *)lcs_malloc(sizeof(int)*(n+1));
    }
 
    // Fill first column and first row of 'C' with 0.
    for(int i = 1; i <= m; i++){c[i][0] = 0;}
    for(int j = 0; j <= n; j++){c[0][j] = 0;}
    
    //For each character in each string
    for(int i = 1; i <= m; i++){
        for(int j = 1; j <= n; j++){
            if (X[i-1] == Y[j-1]){
                c[i][j] = c[i-1][j-1] + 1;
                b[i][j] = DIAG;
            }
            else if (c[i-1][j] >= c[i][j-1]){
                c[i][j] = c[i-1][j];
                b[i][j] = UP;
            }
            else{
                c[i][j] = c[i][j-1];
                b[i][j] = LEFT;
            }
        }
    }
    

    //Debug Statements.
    //printf("Length of LCS: %d \n", c[m][n]);

    //Do we need the io here?
    //printf("Length of LCS: %d \n", c[m][n]);
    

    //Free C and B
    for(int i=0; i <= m; i++) {
        free(c[i]);
    }
    free(c);
    for(int i=0; i <= m; i++) {
        free(b[i]);
    }
    free(b);
    
    return 0; // LCS needs to return number of recursions.
}

//This main assumes that str1 is in argv[1]
//and str2 is in argv[2]
// This is ensured by the main script
int main(int argc, char** argv){
   
    int ittr, x, y;
    sscanf (argv[1], "%i", &ittr);
   
    scanf("%d %d", &x, &y);
    char* a = (char*)malloc(x * sizeof(char)+1);
    char* b = (char*)malloc(y * sizeof(char)+1);
    char* ansref = (char*)malloc( x * sizeof(char) );
    
    scanf("%s %s", a, b);
    
    //printf("%s\n%s\n", a,b);

    printf("Timing, Dynamic Programming implementation:\n");
    timeit( lcs_dyn_prog, ittr, a, b, ansref );
    extern int memusage;
    printf("Dynamic Memory Allocated: %d bytes\n", memusage/ittr);
    free( ansref );

    return 0;
}

//Test main
/*
int main(){
    char a[4] = {'a','c','b',0};
    char b[5] = {'a','a','b','b',0};
    lcs_rec(a,b);
    
    return 0;
}
*/


