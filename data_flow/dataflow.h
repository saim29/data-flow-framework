
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


/*

Implementation Notes:
- Value numbering to setup bitvectors

*/

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.

    class dataFlow {

        public:
        bool direction; // 0 backward; 1 forward
        bool meetOp; // 0 intersection; 1 Union
        
        BitVector transferFunc(BitVector in);
        

    };

    

}

#endif
