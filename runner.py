#!/usr/bin/env python
__usage__ = "%(prog)s [-h] [-v] [-a] [-s N ..] [-l] [-d N] ( -r [[Min] Max] A | -i | -c S S)"
__version__ = "0.1"
__description__ = """Runner script for the Fundamentals of Algorithms
group project for timing and analyzing LCS functions.
"""

import os
import re
import Queue
import random
import signal
import argparse
import threading
from subprocess import Popen, PIPE
from output_formater import print_longstring, print_table

AlgorithmList = ["Naive Recursive algorithm",
                 "Memoized Recursive algorithm", 
                 "Dynamic Programming algorithm",
                 "Quadratic-time linear-space algorithm" ];

def setup_argparse():
    prsr = argparse.ArgumentParser( description=__description__,
                                    version=__version__ )
    prsr.add_argument('-a', '--all', action='store_true', help="Run all LCS versions in sequence.")
    prsr.add_argument('-s', '--select',metavar='N',default=[1,2,3,4], nargs='+', type=int, help="Pass a list of tests to run by number.")
    prsr.add_argument('-d', '--ittr', action='store', default=100, metavar='N',type=long, help="The number of times to iterate")
    prsr.add_argument('--dbg', action='store_true', help="Turn on valgrind support to test for memory leaks.")

    # Can either input via std or on command line, but not both.
    inputGroup = prsr.add_mutually_exclusive_group(required=True)
    inputGroup.add_argument('-l', '--list', action='store_true', help="List runnable tests.")
    inputGroup.add_argument('-m', '--maxrun', action='store_true', help="Get the max character lengths each algorithm can run in 10s.")
    inputGroup.add_argument('-i', '--stdin', action='store_true', help="Pipe strings via stdin rather than on command line.")
    inputGroup.add_argument('-r', '--random', nargs='+', default=[], help="Gen two random strings, within lengths Min and Max of alphabet.")
    inputGroup.add_argument('-c', '--strings', nargs=2, metavar='S',type=str,help="Pass strings on the command line rather than via stdin.")
    
    return prsr

def list_tests():
    print "The following are the Versions of LCS we have available:"
    for i in range( len( AlgorithmList ) ):
        print "\t",(i+1)," - ", AlgorithmList[i]

def sample( l, c ):
    return "".join([random.choice(l) for _ in range(c)])

def run( index, itter, str1, str2, dbg = False, timer=False ):
    def kill_on_timeout( p ):
        if p.poll() == None:
            try: 
                os.killpg( p.pid, signal.SIGTERM )
                print "killed"
            except: pass

    def target(res):
        v = "valgrind " if dbg else ""
        p = Popen( v+"./lcs{0} {1}".format( index, itter ), 
                   stdin=PIPE, stdout=PIPE, 
                   shell=True, preexec_fn=os.setsid )
        if timer: t = threading.Timer( 11.0, kill_on_timeout, [p])
        p.stdin.write( "{0} {1}\n{2} {3}".format( len(str1), len(str2),  
                                              str1, str2 ) )
        if timer: t.start()
        try:
            res.put( p.communicate() )
        except: pass
        finally:
            if timer: t.cancel()
    
    res = Queue.Queue( 1 )
    thread = threading.Thread(target=target, args=[res])
    thread.start()
    thread.join()

    if res.empty(): return None
    else: return res.get()

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

def get_runtime( out ):
    match = re.search(r"\(in ms\):\s?([.\d]+)", out)
    if match:
        time = match.group(1)
        return float(time)
    else:
        return 999.0 # Timeout possibility.

def find_max( testID ):
    threshold = 10; # 10 Seconds.
    curOut = 0;
    curStart = [23,23000,23000,23000][testID-1]; # Higher up means quicker. 
    s1,s2 = random_strs( [ curStart-1, "01" ] )
    update = lambda x,y: (x+"1",y+"1") # TODO: need a better update function.
    while curOut < threshold:
        s1,s2=update(s1,s2)
        print "running with",len(s1),",",len(s2)
        x = run( testID, 1, s1, s2 , timer=True )
        if x == None:
            print "x was none"
            curOut = 9999
        else:
            (out,_) = x
            curOut = get_runtime(out)
    return len(s1)-1

def run_maxruntest( args ):
    global AlgorithmList
    runlist = args.select
    if( args.all ):
        runlist = [1,2,3,4]
    maxlist = []
    for index in runlist:
        m = find_max(index)
        print "Max Input size for",AlgorithmList[index-1],"is",m
        maxlist.append( [m] )
    print_table( maxlist, column_names=['InputSize'])

def main():
    pr = setup_argparse()
    args = pr.parse_args()

    if( args.list ):
        list_tests()
        return
    if( args.maxrun ):
        run_maxruntest( args )
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
         (out, err) = run( t, itter, str1, str2, args.dbg )
         print out
   
if __name__ == "__main__": main();
