LCSExperiment
=============

To Build: `make`

To Run, type: `./runner.py -h` for options.

    usage: runner.py [-h] [-v] [-a] [-s N [N ...]] [-l] [-d N] (-i | -c S S)

    Runner script for the Fundamentals of Algorithms group project for timing
    and analyzing LCS functions.

    optional arguments:
      -h, --help            show this help message and exit
      -v, --version         show program's version number and exit
      -a, --all             Run all LCS versions in sequence.
      -s N [N ...], --select N [N ...]
                            Pass a list of tests to run by number.
      -l, --list            List runnable tests.
      -d N, --ittr N        The number of times to iterate
      -i, --stdin           Pipe strings via stdin rather than on command line.
      -c S S, --strings S S
                            Pass strings on the command line rather than via
                            stdin.

Example Usage:

* Testing only LCS version 2 and 3 on two simple strings:
    
    `./runner.py -s 2 3 -c aabb abab`

* Testing all LCS versions on two strings delimited by a new line in a file 
called 'test_input':

    `cat test_input | ./runner.py -a -i`

