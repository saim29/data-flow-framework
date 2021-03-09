
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {

  // Add code for your dataflow abstraction here.

  DFF::DFF() {

  }

  void DFF::setBoundary(bool direction, bool boundary_val, unsigned bitvec_size) {

    BitVector init_val = BitVector(bitvec_size, boundary_val);

    if(direction == 0) { // Forwards
      out[&F->getEntryBlock()] = init_val;
    }

    else {  // Backwards
      for (auto ele: getPossibleExitBlocks()) {
        out[ele] = init_val;
      }
    }
  }

  DFF::DFF(Function *F, bool direction, meetOperator meetOp,  unsigned bitvec_size, transferFuncTy transferFunc, bool boundary_val) {

    this->F = F;
    this->direction = direction;
    this->meetOp = meetOp;
    this->transferFunc = transferFunc;
    this->T.resize(bitvec_size, false);
    this->B.resize(bitvec_size, false);
    
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

    // initial value of in and out 
    for (BasicBlock &B : *F) {

      in[&B] = T;
      out[&B] = T;

    }

    // Boundary condition

    setBoundary(direction, boundary_val, bitvec_size);



  }

  DFF::~DFF() {
    
  }  

  void DFF::setGen(BBVal gen) {

    this->gen = gen;

  }

  void DFF::setKill(BBVal kill) {

    this->kill = kill;

  }

  BBList DFF::getPossibleExitBlocks() {

    BBList ret;

    for (BasicBlock &B: *F) {

      for (Instruction &I: B) {

        if (dyn_cast<ReturnInst>(&I)) {
          ret.push_back(&B);
          break;
        }
      }
    }
    return ret;
  }

  BitVector DFF::applyMeet(BitVector b1, BitVector b2) {

    BitVector output;

    if (meetOp == INTERSECTION) {

      output = set_intersection(b1, b2);

    } else if (meetOp == UNION) {

      output = set_union(b1, b2);
    }
    return output;
  }

  void DFF::runAnalysis() {

    if (direction) {

      // backward analysis
      // need exit block here but LLVM does not have an explicit exit block
      BBList exitBlockPreds = getPossibleExitBlocks();
      bool changed = false;

      do {

        changed = false;
        std::queue<BasicBlock*> bfs;
        DenseSet<BasicBlock*> visited;
        
        for(auto ele : exitBlockPreds) {

          bfs.push(ele);

        }

        while (!bfs.empty()) {

          BasicBlock *curr = bfs.front();
          bfs.pop();

          visited.insert(curr);


          // apply meet operator here
          for (BasicBlock *succ : successors(curr)) {

            out[curr]= applyMeet(out[curr], out[succ]);

          }

          // push all succcessors in the queue
          for (BasicBlock *pred : predecessors(curr)) {

            if (visited.find(pred) == visited.end()) {

              bfs.push(pred);

            }
          }

          BitVector old_in = in[curr];
          BitVector old_out = out[curr];

          // call transfer function
          BitVector new_in = transferFunc(out[curr], gen[curr], kill[curr]);

          // compare the new in and out to the old ones
          if (new_in != old_in) 
            changed = true;

        }

      } while (changed);


    } else {

      //reverse post order for fastest convergence. Can implement this too
      // const BasicBlock *entry = &F.getEntryBlock();

      // for (po_iterator<BasicBlock*> I = po_begin(&F.getEntryBlock()), IE = po_end(&F.getEntryBlock()); I != IE; ++I) {
      
      // }

      // forward analysis
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

          // apply meet operator here
          for (BasicBlock *pred : predecessors(curr)) {

            in[curr]= applyMeet(in[curr], in[pred]);

          }

          // push all succcessors in the queue
          for (BasicBlock *succ : successors(curr)) {

            if (visited.find(succ) == visited.end()) {

              bfs.push(succ);

            }
          }

          BitVector old_in = in[curr];
          BitVector old_out = out[curr];

          // call transfer function
          BitVector new_out = transferFunc(in[curr], gen[curr], kill[curr]);

          // compare the new in and out to the old ones
          if (new_out != old_out) 
            changed = true;

        }

      } while (changed);
    }
  }

  template<typename A> void DFF::printRes(std::map<A, unsigned> mapping) {

    A rev_mapping[mapping.size()];

    for (auto ele : mapping) {

      unsigned ind = ele->second;
      A val = ele->first;

      rev_mapping[ind] = val;

    }

    for (BasicBlock &B: *F) {

      StringRef bName = B.getName();

      outs () << "==============" + bName + "==============" << "\n";

      outs () << "\nIN: " << bName << "\n";
      print(in[&B], rev_mapping);

      outs () << "\nGEN: " << bName << "\n";
      print(gen[&B], rev_mapping);

      outs () << "\nKILL: " << bName << "\n";
      print(kill[&B], rev_mapping);

      outs () << "\nOUT: " << bName << "\n";
      print(out[&B], rev_mapping);

      outs () << "\n============================" << "\n";

    }

  }

  template<typename A> void DFF::print(BitVector b, A rev_mapping[]) {

    for (int i=0; i<b.size(); i++) {

      if (b[i]) {
        if(!std::is_same<A, Expression>::value){
          outs() << rev_mapping[i] << "\n";
        }
          
        else {
          Expression exp = rev_mapping[i];
          outs() << exp.toString() << "\n";
        }

      }
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
