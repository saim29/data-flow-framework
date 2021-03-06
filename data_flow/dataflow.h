
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

// can add support for more operators
enum meetOperator {

    INTERSECTION,
    UNION

};

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.

    class dataFlow {

        private:
        bool direction; // 0 forward; 1 backward
        meetOperator meetOp;

        // can use StringRef instead of BasicBlock*
        ValueMap<BasicBlock*, BitVector> in; //in[B]
        ValueMap<BasicBlock*, BitVector> out; //out[B]

        BitVector T; //Top value of the semi lattice
        BitVector B; //Bottom value of the semi lattic

        public:

    };

    

}

#endif
