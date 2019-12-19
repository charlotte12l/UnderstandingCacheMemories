# CSAPP Cache Lab: Understanding Cache Memories

## Running the autograders:

- Before running the autograders, compile your code:

    linux> make

- Check the correctness of your simulator:

    linux> ./test-csim

- Check the correctness and performance of your transpose functions:

    linux> ./test-trans -M 32 -N 32

    linux> ./test-trans -M 64 -N 64
    
    linux> ./test-trans -M 61 -N 67

- Check everything at once (this is the program that your instructor runs):
    
    linux> ./driver.py    

## Files:

### My Modiforcation for Handout
- csim.c       My cache simulator
- trans.c      My transpose function

### Tools for evaluating your simulator and transpose function
- Makefile     Builds the simulator and tools
- README       This file
- driver.py*   The driver program, runs test-csim and test-trans
- cachelab.c   Required helper functions
- cachelab.h   Required header file
- csim-ref*    The executable reference cache simulator
- test-csim*   Tests your cache simulator
- test-trans.c Tests your transpose function
- tracegen.c   Helper program used by test-trans
- traces/      Trace files used by test-csim.c

## My Results
```
$ ./driver.py
Part A: Testing cache simulator
Running ./test-csim
                        Your simulator     Reference simulator
Points (s,E,b)    Hits  Misses  Evicts    Hits  Misses  Evicts
     3 (1,1,1)       9       8       6       9       8       6  traces/yi2.trace
     3 (4,2,4)       4       5       2       4       5       2  traces/yi.trace
     3 (2,1,4)       2       3       1       2       3       1  traces/dave.trace
     3 (2,1,3)     167      71      67     167      71      67  traces/trans.trace
     3 (2,2,3)     201      37      29     201      37      29  traces/trans.trace
     3 (2,4,3)     212      26      10     212      26      10  traces/trans.trace
     3 (5,1,5)     231       7       0     231       7       0  traces/trans.trace
     6 (5,1,5)  265189   21775   21743  265189   21775   21743  traces/long.trace
    27


Part B: Testing transpose function
Running ./test-trans -M 32 -N 32
Running ./test-trans -M 64 -N 64
Running ./test-trans -M 61 -N 67

Cache Lab summary:
                        Points   Max pts      Misses
Csim correctness          27.0        27
Trans perf 32x32           8.0         8         287
Trans perf 64x64           8.0         8        1219
Trans perf 61x67          10.0        10        1847
          Total points    53.0        53
```
