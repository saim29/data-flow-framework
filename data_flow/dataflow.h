
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

// included for convenience
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/DenseSet.h"

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

        Function *F; // pointer to the function under inspection

        bool direction; // 0 forward; 1 backward
        meetOperator meetOp; // meet operator for preds or succ

        // can use StringRef instead of BasicBlock*
        VMap bvec_mapping; // maps the domain to the indexes in the bitmap
        BBVal in; // in[B]
        BBVal out; // out[B]

        BitVector T; // Top value of the semi lattice
        BitVector B; // Bottom value of the semi lattice

        BitVector (*transfer)(BitVector, BitVector, BitVector); // function pointer to the transfer function of the analysis class

        void traverseCFG(); // traversal of basicblocks based on the direction boolean

        public:
        // constructors for DFF
        DFF();
        DFF(Function *F, bool direction, meetOperator meetOp, BitVector(*transfer)(BitVector, BitVector, BitVector), unsigned bitvec_size);

        // destructor for DFF
        ~DFF();

    };
}

#endif
