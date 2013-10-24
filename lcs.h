//
//  lcs.h
//  Multiple Implemetations of the LCS algorithm
//
//  Authors: Pat Gillis and Alex Dean
//

#ifndef _lcs_h
#define _lcs_h

#define MAX(X,Y) ((X) > (Y) ? (X) : (Y))
#define DIAG 1
#define UP 2
#define LEFT 3

void lcs_rec(char* X, char* Y);
void print_lcs(int** b, char* X, int i, int j);

#endif
