
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {

  // Add code for your dataflow abstraction here.

  DFF::DFF() {

  }

  DFF::DFF(Function *F, bool direction, meetOperator meetOp, BitVector(*transfer)(BitVector, BitVector, BitVector), unsigned bitvec_size) {

    this->F = F;
    this->direction = direction;
    this->meetOp = meetOp;
    this->transfer = transfer;
    this->T.resize(bitvec_size);
    this->B.resize(bitvec_size);
    
    // initialize top and bottom elements of the semi-lattice
    if (meetOp == INTERSECTION) {

      for(int i=0; i<bitvec_size; i++) {

        T[i] = 1;
        // B[i] = 0;

      }

    } else if (meetOp == UNION) {

      for(int i=0; i<bitvec_size; i++) {

        // T[i] = 0;
        B[i] = 1;

      }
    }

    for (BasicBlock &B : *F) {

      if (direction) { 



      } else {


      }
      in[&B] = T;
      out[&B] = T;

    }
  }

  void DFF::traverseCFG() {

    if (direction) {

      // backward analysis

    } else {

      //reverse post order for fastest convergence

      // forward analysis
      // const BasicBlock *entry = &F.getEntryBlock();

      // for (po_iterator<BasicBlock*> I = po_begin(&F.getEntryBlock()), IE = po_end(&F.getEntryBlock()); I != IE; ++I) {
      
      // }


      // breadth-first search solution

      BasicBlock *entry_block = &F->getEntryBlock();

      bool changed = false;

      do {

        changed = false;
        std::queue<BasicBlock*> bfs;
        DenseSet<BasicBlock*> visited;
        bfs.push(entry_block);

        while (!bfs.empty()) {

          BasicBlock *curr = bfs.front();
          bfs.pop();

          visited.insert(curr);

          // push all succcessors in the queue
          for (BasicBlock *succ : successors(curr)) {

            if (visited.find(succ) == visited.end()) {

              bfs.push(succ);

            }
          }

          BitVector old_in = in[curr];
          BitVector old_out = out[curr];

          // call transfer function

          // compare the new in and out to the old ones

          // changed = true if they changed otherwise continue;
        }

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
