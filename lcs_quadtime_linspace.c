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
void algorithmB( int m, int n, char* A, char* B, iMAT_ROW* L );
int algorithmC( int m, int n, char* A, char* B, char* C );
int Hirshberg( char* A, char* B, char* LCS );
void strrev( char* s );
void strsplit( int len, int i, const char* s, char* f, char* b );
void blankC( int max, char* C );

void debug(const char* msg){
    printf(msg);fflush(stdout);
}
void debug_printarray( const int l, const int* a ){
    printf("[");
    for( int i=0; i< l; i++){
        printf("%d,",a[i]);
    }
    printf("]\n");fflush(stdout);
}

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
void algorithmB( int m, int n, char* A, char* B, iMAT_ROW* LL ){
    // Initialize temporary K table (size 2xN).
    iMAT K = (iMAT)lcs_malloc( 2 * sizeof(iMAT_ROW) );
    K[0] = (iMAT_ROW)lcs_malloc( (n+1) * sizeof(int) );
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
    *LL = K[1];
}

int algorithmC( int m, int n, char* A, char* B, char* C ){
    debug("Start C:\n");
    blankC( m, C ); 
    debug("\tMade C\n");

    // If trivial then solve:
    if( n == 0 ){ 
        debug("\t<=Default Case\n");
        return 0; 
    }
    if( m == 1 ){ 
        debug("\t<=M Case: ");
        for(int i=0; i<n; i++){
            if(A[0] == B[i]){
                char ch[2] = { B[i], '\0' }; 
                debug(ch);
                strcat(C, ch); 
                return 1; 
            }
        }
        debug("\n");
        return 0;
    }
    
    // We want to half the problem each step.
    int i = m/2;

    // Break up A and reverse second half of A and B completely
    char *A_1i, A_mi1[m-i+1], *A_i1m; 
        strsplit( m, i, A, A_1i, A_i1m );
        debug("\t A Split successful\n");
        strcpy(A_mi1, A_i1m);
        debug("\t A Duplicate successful\n");
        strrev( A_mi1 );
        debug("\t A Reverse successful\n");
    char  B_n1[n]; 
        strcpy(B_n1, B); 
        strrev( B_n1 );
        debug("\t B Reverse successful\n");

    // Run Algorithm B over the broken up strings. We allocate a table for
    // the results. Each call returns a row.
    iMAT L = (iMAT)lcs_malloc( 2 * sizeof(iMAT_ROW) );
    debug("\t L table creation successful\n");
    algorithmB( i,   n, A_1i,  B,    &L[0] ); debug("\t First Algo B done.\n");
    algorithmB( m-i, n, A_mi1, B_n1, &L[1] ); debug("\t Second Algo B done.\n");

    // We calculate the Max LCS size for each sub string separately.
    int k=0,M=0;
    debug("\t MAX Calculation\n");
    debug_printarray(n+1,L[0]);debug_printarray(n+1,L[1]);
    for(int j=0; j <= n; j++){ 
        debug("\t\t M = ");
        M = MAX( M, L[0][j] + L[1][n-j] ); //SEGFAULTING HERE
        printf("%d\n",M);fflush(stdout);
    }
    debug("\t K Calculation\n");
    for(int j=0; j <= n; j++){ if(L[0][j]+L[1][n-j] == M){ k=j; break;} }

    // Recursively check each substring and then concatenate the results.
    char *C1, *C2; int a=0,b=0;
    char *B_1k, *B_k1n; 
    debug("\t BK Split\n");
    strsplit( n, k, B, B_1k, B_k1n );

    debug("\t Starting recursion: ----\n");
    a = algorithmC( i, k, A_1i, B_1k, C1 ); debug("\t Finishing recursion1-----\n");
    b = algorithmC( m-i, n-k, A_i1m, B_k1n, C2 );debug("\t Finishing recursion2-----\n");
    
    // Concat results and return.
    char T[a+b];
    strcat( T, C1 );
    strcat( T, C2 );
    C = T;
    debug("concat");
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

    debug("Running Hirshberg.\n");

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

