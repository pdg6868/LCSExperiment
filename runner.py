#!/usr/bin/env python
__usage__ = "%(prog)s [-h] [-v] [-a] [-s N ..] [-l] [-d N] (-i | -c S S)"
__version__ = "0.1"
__description__ = """Runner script for the Fundamentals of Algorithms
group project for timing and analyzing LCS functions.
"""

import os
import argparse

def setup_argparse():
    prsr = argparse.ArgumentParser( description=__description__,
                                    version=__version__ )
    prsr.add_argument('-a', '--all', action='store_true', help="Run all LCS versions in sequence.")
    prsr.add_argument('-s', '--select',metavar='N',default=[1], nargs='+', type=int, help="Pass a list of tests to run by number.")
    prsr.add_argument('-l', '--list', action='store_true', help="List runnable tests.")
    prsr.add_argument('-d', '--ittr', nargs=1, default=100, metavar='N',type=long, help="The number of times to iterate")

    # Can either input via std or on command line, but not both.
    inputGroup = prsr.add_mutually_exclusive_group(required=True)
    inputGroup.add_argument('-i', '--stdin', action='store_true', help="Pipe strings via stdin rather than on command line.")
    inputGroup.add_argument('-c', '--strings', nargs=2, metavar='S',type=str,help="Pass strings on the command line rather than via stdin.")
    
    return prsr

def list_tests():
    print """The following are the Versions of LCS we have available:
    1 - Naive Recursive algorithm
    2 - Memoized Recursive algorithm
    3 - Dynamic Programming algorithm
    4 - Quadratic-time linear-space algorithm
    """

def main():
    pr = setup_argparse()
    args = pr.parse_args()

    if( args.list ):
        list_tests()
        return

    itter   = args.ittr
    runlist = args.select
    if( args.all ):
        runlist = [1,2,3,4]

    if args.stdin:
        str1,str2 = (raw_input(),raw_input())
    else:
        str1,str2 = tuple(args.strings)

    for t in runlist:
        os.system("./lcs{0} {1} {2} {3}".format(t,itter,str1,str2))

   
if __name__ == "__main__": main();
