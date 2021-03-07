
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

namespace llvm {

    // Add definitions (and code, depending on your strategy) for your dataflow
    // abstraction here.

    // can add support for more meet operators here
    enum meetOperator {

        INTERSECTION,
        UNION

    };

    typedef ValueMap <BasicBlock*, BitVector> BBVal;
    typedef ValueMap <Value*, unsigned> VMap;

    class DFF {

        private:

        // constructors for DFF
        DFF();

        // destructors for DFF
        ~DFF();

        bool direction; // 0 forward; 1 backward
        meetOperator meetOp; // meet operator for preds or succ

        // can use StringRef instead of BasicBlock*
        VMap bvec_mapping; // maps the domain to the indexes in the bitmap
        BBVal in; // in[B]
        BBVal out; // out[B]

        BitVector T; // Top value of the semi lattice
        BitVector B; // Bottom value of the semi lattice

        BitVector set_op(BitVector b1, BitVector b2, meetOperator meetOp); // function for set operations such as union or intersection
        void traverseCFG(); // traversal of basicblocks based on the direction boolean

        public:
        void (*transfer)(void);


    };

    

}

#endif
