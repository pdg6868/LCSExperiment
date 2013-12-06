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

// So we can see mutable functions.
#define CSTR char*

// Inline functions
#define MAX( X, Y ) ((X) > (Y) ? (X) : (Y))

void algorithmA( int m, int n, CSTR A, CSTR B, iMAT* L );
void algorithmB( int m, int n, CSTR A, CSTR B, iMAT_ROW* L );
int algorithmC( int m, int n, CSTR A, CSTR B, CSTR* C );
int Hirshberg( CSTR A, CSTR B, int m, int n, CSTR* LCS );
void strrev( CSTR s );
void strsplit( int len, int i, CSTR s, CSTR* f, CSTR* b );

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
void algorithmA( int m, int n, char* A, char* B, iMAT* L ){
    iMAT LL = *L;

    // Initialize L Table.
    for( int i=0; i <= m; i++ ){ LL[i][0]=0; }
    for( int j=0; j <= n; j++ ){ LL[0][j]=0; }

    // Creation of L table.
    for( int i=1; i <= m; i++ ){
        for(int j=1; j <= n; j++ ){
            if( A[i-1] == B[j-1] ){
                LL[i][j] = LL[i-1][j-1]+1;
            }else{
                LL[i][j] = MAX( LL[i][j-1], LL[i-1][j] );
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
void algorithmB( int m, int n, CSTR A, CSTR B, iMAT_ROW* LL ){
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
                K[1][j] = K[0][j-1] + 1;
            }else{
                K[1][j] = MAX( K[1][j-1], K[0][j] );
            }
        }
    }
 
    // Note Return is just a single Row.
    *LL = K[1];
  
    // Clean up allocated memory. 
    free(K[0]);
    // Free Pointer to table.
    free( K );
}

/** 
 * Algoithm C takes two strings and their lengths and 
 * produces the actual LCS. It will also return the number
 * of (recursions that are taken).
 **/
int algorithmC( int m, int n, CSTR A, CSTR B, CSTR* C ){
    // If trivial then solve:
    if( n == 0 ){ return 0; }
    if( m == 1 ){ 
        for(int i=0; i<n; i++){
            if(A[0] == B[i]){
                const char ch[] = { B[i], '\0' }; 
                strcat(*C, ch);
                break; 
            }
        }
        return 0;
    }
    
    // We want to half the problem each step.
    int i = m/2;

    // Break up A and reverse second half of A and B completely
    char *A_1i, A_mi1[m-i+1], *A_i1m; 
        strsplit( m, i, A, &A_1i, &A_i1m );
        strcpy(A_mi1, A_i1m);
        strrev( A_mi1 );
    char B_n1[n]; 
        strcpy(B_n1, B); 
        strrev( B_n1 );

    // Run Algorithm B over the broken up strings. We allocate a table for
    // the results. Each call returns a row.
    iMAT L = (iMAT)lcs_malloc( 2 * sizeof(iMAT_ROW) );
    algorithmB( i,   n, A_1i,  B,    &L[0] ); 
    algorithmB( m-i, n, A_mi1, B_n1, &L[1] ); 

    // We calculate the Max LCS size for each sub string separately.
    int k=0,M=0;
    for(int j=0; j <= n; j++){ M = MAX( M, L[0][j] + L[1][n-j] ); }
    for(int j=0; j <= n; j++){ if(L[0][j]+L[1][n-j] == M){ k=j; break;} }
    free(L[0]); free(L[1]); free(L);

    // Recursively check each substring and then concatenate the results.
    char *B_1k, *B_k1n; strsplit( n, k, B, &B_1k, &B_k1n );

    int rec = algorithmC( i, k, A_1i, B_1k, C )
            + algorithmC( m-i, n-k, A_i1m, B_k1n, C );

    free( A_1i ); free( A_i1m );
    free( B_1k ); free( B_k1n );

    // Return the number of recursive steps
    return rec+2;
}

/** In-place string reversal. Goes from either side to the middle. */
void strrev( CSTR p ){
    char *q = p;
    while(q && *q) ++q;
    for(--q; p < q; ++p, --q)
        *p = *p ^ *q,
        *q = *p ^ *q,
        *p = *p ^ *q;
}

/** Simple string splitting algorithm. */
void strsplit( int len, int i, CSTR s, CSTR* front, CSTR* back ) {
    int bl = len-i+1;
    char* bs = s+( i*sizeof(char) );
    
    *front = (char*)lcs_malloc( (i+1)*sizeof(char) ); 
    memcpy( *front, s, i*sizeof(char) );
    (*front)[i] = '\0';
    
    *back = (char*)lcs_malloc( bl*sizeof(char) );
    memcpy( *back, bs, bl*sizeof(char) ); // NULL char comes free from s.
}

/**
 * A wrapper function for Algorithm C so that we can pass it 
 * nicely into the timeit function. We have overridden algorithm
 * C to return the number of recursions it performs instead of
 * the LCS length. If we wanted to find LCS length at this point
 * we can check C.
 **/
int Hirshberg( char* A, char* B, int m, int n, char** C ){
    int r; // Number of recursions.
    strcpy(*C, ""); // Initialize C so we know where to concat strings.

    // The Larger string should be in the A position.
    if( m >= n ) {
        r = algorithmC( m, n, A, B, C );
    }else{
        r = algorithmC( n, m, B, A, C );
    }

//    debug("Found LCS is = "); debug(*C); debug("\n");

    return r;
}

/**
 * A main runner function that runner.py uses to execute LCS
 * tests with the Hirshberg algorithm.
 **/
int main( int argc, char** argv ){
    
    int ittr, x, y;

    extern ulong memusage;
    memusage = 0L;

    sscanf( argv[1], "%i", &ittr);
    scanf("%d %d", &x, &y);

    char* a = (char*)malloc( (x+1) * sizeof(char) );
    char* b = (char*)malloc( (y+1) * sizeof(char) );
    char* ansref = (char*)malloc( (x+1) * sizeof(char) );

    scanf( "%s %s", a, b );

    printf("Timing, Quadratic Time and Linear Space Algorithm (Hirshberg's):\n");
    double avg = timeit( Hirshberg, ittr, a, b, x, y, &ansref );

    printf( "Dynamic Memory Allocated: %lu bytes\n", memusage/ittr );
    printf( "Time Const: %e\n", avg / (x+y) );// Quadratic, so sum.

    free( a ); free( b );
    free( ansref );
    return 0;
}

