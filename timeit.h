/**
 * Timing Functionality 
 *
 * timeit - will itteratively call an LCS function and return the length of time
 *      it took to call it 'n' times.
 **/

// CPUTIME MACRO PROVIDED BY PROFESSOR -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
#include <sys/time.h>
#include <sys/resource.h>
extern int getrusage();
#define CPUTIME (getrusage(RUSAGE_SELF,&ruse),\
          ruse.ru_utime.tv_sec + ruse.ru_stime.tv_sec + \
          1e-6 * (ruse.ru_utime.tv_usec + ruse.ru_stime.tv_usec))
struct rusage ruse;
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

/*********************************************************************
 * Times an LCS function by repeatedly calling it `ittr` times with 
 * the inputs `s1` and `s2`. It will return the difference in time, 
 * and leaves averaging up to the caller. ASSUMES VOID RETURN TYPE.
 *********************************************************************/
double timeit( int (*lcs)(char*,char*),
               int ittr, 
               char* s1, char* s2 )
{
    char* ans;
    double start, end;
    int i=0;
    
    // Loop over lcs calculation.
    start = CPUTIME;
    while( i<ittr ){
        lcs(s1, s2);
        i++;
    }
    end = CPUTIME;

    // Memory management.
    //delete[] ans;

    // Return difference in seconds
    return (end-start);
}

