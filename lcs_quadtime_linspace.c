//
// lcs_quadtime_linspace.c
// This is the default Hirshberg Algorithm which computes LCS in 
// Quadratic Time and Linear Space.
//
// Authors Alex Dean and Pat Gillis
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lcs.h"
#include "timeit.h"

// Useful integer matrix type
#define iMAT int**
#define iMAT_ROW int*

// Inline functions
#define MAX( X, Y ) ((X) > (Y) ? (X) : (Y))

void algorithmA( int m, int n, char* A, char* B, iMAT L );
void algorithmB( int m, int n, char* A, char* B, iMAT_ROW L );
int algorithmC( int m, int n, char* A, char* B, char* C );
int Hirshberg( char* A, char* B, char* LCS );
void strrev( char* s );
void strsplit( int len, int i, const char* s, char* f, char* b );
void blankC( int max, char* C );

/**
 * Algorithm A takes two strings and creates table L which 
 * contains the max length possible of any common subsequence
 * at that location. Note L[m][n] would be our max lcs length.
 *
 * Time Complexity: O(mn)
 * Space Complexity: O(mn)
 *
 * Note AlgorithmB is a better implementation of this and does 
 * not need as much space. We do not use this algorithm, it is
 * here for completeness sake. 
 **/
void algorithmA( int m, int n, char* A, char* B, iMAT L ){
    // Initialize L Table.
    for( int i=0; i <= m; i++ ){ L[i][0]=0; }
    for( int j=0; j <= n; j++ ){ L[0][j]=0; }

    // Creation of L table.
    for( int i=1; i <= m; i++ ){
        for(int j=1; j <= n; j++ ){
            if( A[i-1] == B[j-1] ){
                L[i][j] = L[i-1][j-1]+1;
            }else{
                L[i][j] = MAX( L[i][j-1], L[i-1][j] );
            }
        }
    }
}


/**
 * Algorithm B takes two strings and finds the length
 * of the max possible LCS between them. Note this is a 
 * drop in replacement of Algorithm A, but has a better
 * Space Complexity signature.
 *
 * Time Complexity: O(mn)
 * Space Complexity: O(m+n)
 **/
void algorithmB( int m, int n, char* A, char* B, iMAT_ROW LL ){
    // Initialize temporary K table (size 2xN).
    iMAT K = (iMAT)lcs_malloc( 2 * sizeof(iMAT_ROW) );
    K[1] = (iMAT_ROW)lcs_malloc( (n+1) * sizeof(int) );
    for( int j=0; j<=n; j++ ){ K[0][j]=0; K[1][j]=0; }
    
    // Fill temporary K table.
    for( int i=1; i <= m; i++ ){
         memcpy(K[0], K[1], (n+1)*sizeof(int));
        for( int j=1; j <= n; j++ ){
            if( A[i-1] == B[j-1] ){
                K[1][j] = K[0][j-1]+1;
            }else{
                K[1][j] = MAX( K[1][j-1], K[0][j] );
            }
        }
    }
   
    // Clean up allocated memory. 
    free(K[0]);

    // Note Return is just a single Row.
    LL = K[1];
}

int algorithmC( int m, int n, char* A, char* B, char* C ){
    blankC( m, C ); 
    
    // If trivial then solve:
    if( n == 0 ){ return 0; }
    if( m == 1 ){ 
        for(int i=0; i<n; i++){
            if(A[0] == B[i]){
                char ch[2] = { B[i], '\0' }; 
                strcat(C, ch); 
                return 1; 
            }
        }
        return 0;
    }
    
    // We want to half the problem each step.
    int i = m/2;

    // Break up A and reverse second half of A and B completely
    char *A_1i, *A_mi1, *A_i1m; 
        strsplit( m, i, A, A_1i, A_i1m );
        strcpy(A_mi1, A_i1m);
        strrev( A_mi1 );
    char  B_n1[n]; 
        strcpy(B_n1, B); 
        strrev( B_n1 );
    
    // Run Algorithm B over the broken up strings. We allocate a table for
    // the results. Each call returns a row.
    iMAT L = (iMAT)lcs_malloc( 2 * sizeof(iMAT_ROW) );
    algorithmB( i,   n, A_1i,  B,    L[0] );
    algorithmB( m-i, n, A_mi1, B_n1, L[1] );

    // We calculate the Max LCS size for each sub string separately.
    int k=0,M=0;
    for(int j=0; j <= n; j++){ M = MAX( M, L[0][j] + L[1][n-j] ); }
    for(int j=0; j <= n; j++){ if(L[0][j]+L[1][n-j] == M){ k=j; break;} }

    // Recursively check each substring and then concatenate the results.
    char *C1, *C2; int a=0,b=0;
    char *B_1k, *B_k1n; strsplit( n, k, B, B_1k, B_k1n );

    a = algorithmC( i, k, A_1i, B_1k, C1 );
    b = algorithmC( m-i, n-k, A_i1m, B_k1n, C2 );
    
    // Concat results and return.
    char T[a+b];
    strcat( T, C1 );
    strcat( T, C2 );
    C = T;
    return a+b;
}

void strrev(char* p){
    char *q = p;
    while(q && *q) ++q;
    for(--q; p < q; ++p, --q)
        *p = *p ^ *q,
        *q = *p ^ *q,
        *p = *p ^ *q;
}

void strsplit( int len, int i, const char* s, char* front, char* back ) {
    int bl = len-i;
    front = malloc( (i+1)*sizeof(char) );
    memcpy( front, s, i );
    front[i] = '\0';
    back = malloc( (bl+1)*sizeof(char) );
    memcpy( back, s, bl );
    back[bl]='\0';
}

void blankC( int max, char* C ){
    C = malloc( (max+1) * sizeof(char) );
    C[0] = '\0';
}

int Hirshberg( char* A, char* B, char* C ){
    int lc; // Length of LCS String.
    
    int m = strlen(A);
    int n = strlen(B);

    // The Larger string should be in the A position.
    if( m > n ) {
        lc = algorithmC( m, n, A, B, C );
    }else{
        lc = algorithmC( n, m, B, A, C );
    }

    // TODO: Should we return the number of recursions instead?
    return lc;
}


int main( int argc, char** argv ){
    
    int ittr, x, y;

    extern int memusage;
    memusage = 0;

    sscanf( argv[1], "%i", &ittr);
    scanf("%d %d", &x, &y);

    char* a = (char*)malloc( (x+1) * sizeof(char) );
    char* b = (char*)malloc( (y+1) * sizeof(char) );
    char* ansref = (char*)malloc( (x+1) * sizeof(char) );

    scanf( "%s %s", a, b );

    printf("Timing, Quadratic Time and Linear Space Algorithm (Hirshberg's):\n");
    double avg = timeit( Hirshberg, ittr, a, b, ansref );

    printf( "Dynamic Memory Allocated: %d bytes\n", memusage/ittr );
    printf( "Time Const: %e\n", avg / (x+y) );// Quadratic, so sum.

    free( ansref );
    return 0;
}

