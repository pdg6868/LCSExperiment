#!/usr/bin/env python
__usage__ = "%(prog)s [-h] [-v] [-a] [-s N ..] [-l] [-d N] ( -r [[Min] Max] A | -i | -c S S)"
__version__ = "0.1"
__description__ = """Runner script for the Fundamentals of Algorithms
group project for timing and analyzing LCS functions.
"""

import random
import argparse
from subprocess import Popen, PIPE
from output_formater import print_longstring

def setup_argparse():
    prsr = argparse.ArgumentParser( description=__description__,
                                    version=__version__ )
    prsr.add_argument('-a', '--all', action='store_true', help="Run all LCS versions in sequence.")
    prsr.add_argument('-s', '--select',metavar='N',default=[1], nargs='+', type=int, help="Pass a list of tests to run by number.")
    prsr.add_argument('-l', '--list', action='store_true', help="List runnable tests.")
    prsr.add_argument('-d', '--ittr', action='store', default=100, metavar='N',type=long, help="The number of times to iterate")

    # Can either input via std or on command line, but not both.
    inputGroup = prsr.add_mutually_exclusive_group(required=True)
    inputGroup.add_argument('-i', '--stdin', action='store_true', help="Pipe strings via stdin rather than on command line.")
    inputGroup.add_argument('-r', '--random', nargs='+', default=[], help="Gen two random strings, within lengths Min and Max of alphabet.")
    inputGroup.add_argument('-c', '--strings', nargs=2, metavar='S',type=str,help="Pass strings on the command line rather than via stdin.")
    
    return prsr

def list_tests():
    print """The following are the Versions of LCS we have available:
    1 - Naive Recursive algorithm
    2 - Memoized Recursive algorithm
    3 - Dynamic Programming algorithm
    4 - Quadratic-time linear-space algorithm
    """

def sample( l, c ):
    return "".join([random.choice(l) for _ in range(c)])

def random_strs( arg ):
    mi,ma,alpha=1,100,"01"
    if len(arg) == 1:
        alpha = arg[0]
    elif len(arg) == 2:
        alpha = arg[1]
        ma = int(arg[0])
        mi = ma
    elif len(arg) == 3:
        alpha = arg[2]
        ma = int(arg[1])
        mi = int(arg[0])
    
    mi=min(mi,ma)
    ma=max(mi,ma)
    alpha = list(alpha)
    da = random.randint(mi, ma)
    db = random.randint(mi, ma)
    s1 = sample( alpha, da ) 
    s2 = sample( alpha, db )
    return (s1, s2)

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

    # Generate Population, either by user or random.
    if args.random:
        if len(args.random) in [1,2,3]:
            str1, str2 = random_strs( args.random )
            print_longstring( 80, 3, str1 )
            print_longstring( 80, 3, str2 )
        else: raise "Random option takes max of three arguments."
    elif args.stdin:
        str1,str2 = (raw_input(),raw_input())
    else:
        str1,str2 = tuple(args.strings)

    for t in runlist:
         p = Popen( "./lcs{0} {1}".format( t, itter), stdin=PIPE, stdout=PIPE, shell=True, close_fds=True )
         p.stdin.write( "{0} {1}\n{2} {3}".format( len(str1), len(str2),  str1, str2 ) )
         (out, err) = p.communicate()
         print out
   
if __name__ == "__main__": main();
