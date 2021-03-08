#! /bin/sh

# $1 = name of pass {liveness, available}
# $2 = name of file {tests/liveness-test-m2r.bc, tests/available-test-m2r.bc}

infile=$2
lib=$1".so"
pass=$1

echo "-> Cleaning last built analysis passes"
make clean

echo "-> Building analysis passes"
make

echo "Obtaining human-readble .ll file ..."
llvm-dis $infile

echo "Running analysis pass ..."
opt -load ./$lib -$pass $infile
