# ECE 5984 Compiler Optimizations - Assignment 2 Programming
An LLVM wrapper pass for implementing different data flow analysis 

## Team Members - Saim Ahmad, Tanmaya Mishra

# Folder Layout:
README.txt
data_flow
| tests
    | available-test-m2r.bc
    | liveness-test-m2r.bc
| available.cpp  
| available-support.cpp 
| available-support.h 
| dataflow.cpp  dataflow.h 
| liveness.cpp 
| Makefile 


# To build and run:

1. Make run.sh executable - `chmod +x run.sh`
2. Synopsis

    ./run.sh [analysis] [input file]
    
    - Input files available under tests 
    - Analysis : available, liveness
   
3. Example output

$ ./run.sh available tests/available-test-m2r.bc 
-> Cleaning last built analysis passes
rm -f *.o *~ *.so
-> Building analysis passes
g++ -rdynamic -I/usr/local/include -std=c++14   -fno-exceptions -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/usr/local/include/ -g -O0 -fPIC   -c -o dataflow.o dataflow.cpp
g++ -rdynamic -I/usr/local/include -std=c++14   -fno-exceptions -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/usr/local/include/ -g -O0 -fPIC   -c -o available-support.o available-support.cpp
g++ -rdynamic -I/usr/local/include -std=c++14   -fno-exceptions -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/usr/local/include/ -g -O0 -fPIC   -c -o liveness.o liveness.cpp
g++ -dylib -shared liveness.o dataflow.o available-support.o -o liveness.so
g++ -rdynamic -I/usr/local/include -std=c++14   -fno-exceptions -D_GNU_SOURCE -D_DEBUG -D__STDC_CONSTANT_MACROS -D__STDC_FORMAT_MACROS -D__STDC_LIMIT_MACROS -I/usr/local/include/ -g -O0 -fPIC   -c -o available.o available.cpp
g++ -dylib -shared available.o dataflow.o available-support.o -o available.so
rm liveness.o available.o
Obtaining human-readble .ll file ...
Running analysis pass ...
********** Function: main ***********
Convergence: 2 iterations
==============entry==============

IN: 


GEN
%argc + 50,  %add + 96,  

KILL


OUT: 
%argc + 50,  %add + 96,  

====================================
==============if.then==============

IN: 
%argc + 50,  %add + 96,  

GEN
%add - 50,  96 * %add,  

KILL


OUT: 
%argc + 50,  %add + 96,  %add - 50,  96 * %add,  

====================================
==============if.else==============

IN: 
%argc + 50,  %add + 96,  

GEN
96 * %add,  %add + 50,  

KILL


OUT: 
%argc + 50,  %add + 96,  96 * %add,  %add + 50,  

====================================
==============if.end==============

IN: 
%argc + 50,  %add + 96,  96 * %add,  

GEN
50 - 96,  %sub4 + %f.0,  

KILL


OUT: 
%argc + 50,  %add + 96,  96 * %add,  50 - 96,  %sub4 + %f.0,  

====================================