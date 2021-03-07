
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {

  // Add code for your dataflow abstraction here.

  DFF::DFF() {

  }

  DFF::DFF(bool direction, meetOperator meetOp, BitVector(*transfer)(BitVector, BitVector, BitVector)) {

    this->direction = direction;
    this->meetOp = meetOp;
    this->transfer = transfer;
    
  }

  void DFF::traverseCFG(Function &F) {

    if (direction) {

      // backward analysis

    } else {

      //reverse post order for fastest convergence

      // forward analysis
      // const BasicBlock *entry = &F.getEntryBlock();

      // for (po_iterator<BasicBlock*> I = po_begin(&F.getEntryBlock()), IE = po_end(&F.getEntryBlock()); I != IE; ++I) {
      
      // }


      // breadth-first search solution

      const BasicBlock *entry_block = &F.getEntryBlock();

      bool changed = false;
      do {

        //traversal {

          // call transfer function
          // check if IN[B] or OUT[B] changed compared to the previous value
          // changed = true if they changed otherwise changed = false

        //}

      } while (changed);

    }
  }

  // definitions of set operations

  BitVector set_union(BitVector b1, BitVector b2) {

    unsigned size = b1.size();
    BitVector u = BitVector(size);

    for (int i=0; i<size; i++) {

        u[i] = b1[i] || b2[i];
    }

    return u;
  }

  BitVector set_intersection(BitVector b1, BitVector b2) {

    unsigned size = b1.size();
    BitVector u = BitVector(size);

    for (int i=0; i<size; i++) {

        u[i] = b1[i] && b2[i];
    }

    return u;

  }

  BitVector set_diff(BitVector b1, BitVector b2) {

    unsigned size = b1.size();
    BitVector u = BitVector(size);

    for (int i=0; i<size; i++) {

        if (b2[i] == 1) {

          u[i] = 0;

        } else {

          u[i] = b1[i];

        }
    }

    return u;

  }
}
