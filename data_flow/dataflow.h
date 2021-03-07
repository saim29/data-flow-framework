
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include <stdio.h>
#include <iostream>
#include <queue>
#include <vector>

#include "llvm/IR/Instructions.h"
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/Pass.h"

#include "llvm/ADT/PostOrderIterator.h"

namespace llvm {

    // Add definitions (and code, depending on your strategy) for your dataflow
    // abstraction here.

    // set operations for bitvectors
    BitVector set_union(BitVector b1, BitVector b2);
    BitVector set_intersection(BitVector b1, BitVector b2);
    BitVector set_diff(BitVector b1, BitVector b2);

    // can add support for more meet operators here
    enum meetOperator {

        INTERSECTION,
        UNION

    };

    typedef ValueMap <BasicBlock*, BitVector> BBVal;
    typedef ValueMap <Value*, unsigned> VMap;

    class DFF {

        private:
        bool direction; // 0 forward; 1 backward
        meetOperator meetOp; // meet operator for preds or succ

        // can use StringRef instead of BasicBlock*
        VMap bvec_mapping; // maps the domain to the indexes in the bitmap
        BBVal in; // in[B]
        BBVal out; // out[B]

        BitVector T; // Top value of the semi lattice
        BitVector B; // Bottom value of the semi lattice

        BitVector (*transfer)(BitVector, BitVector, BitVector); // function pointer to the transfer function of the analysis class

        void traverseCFG(Function &F); // traversal of basicblocks based on the direction boolean

        public:
        // constructors for DFF
        DFF();
        DFF(bool direction, meetOperator meetOp, BitVector(*transfer)(BitVector, BitVector, BitVector));

        // destructor for DFF
        ~DFF();

    };
}

#endif
// Mapping of the IN and OUT bitvectors to the basic block

//        ValueMap<BasicBlock*, BitVector> in; //in[B]
//        ValueMap<BasicBlock*, BitVector> out; //out[B]



//        BitVector T; //Top value of the semi lattice
//        BitVector B; //Bottom value of the semi lattice
        