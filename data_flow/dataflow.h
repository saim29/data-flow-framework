
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

// included for convenience
#include "llvm/ADT/PostOrderIterator.h"
#include "llvm/ADT/DenseSet.h"
#include "llvm/IR/DerivedTypes.h"

#include "available-support.h"

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

    typedef DenseMap <BasicBlock*, BitVector> BBVal;
    typedef std::map <Value*, unsigned> VMap;
    typedef std::map <Expression, unsigned> EMap;
    typedef std::vector<BasicBlock*> BBList;
    typedef BitVector (*transferFuncTy) (BitVector, BitVector, BitVector);

    class DFF {

        private:

        Function *F; // pointer to the function under inspection

        bool direction; // 0 forward; 1 backward
        meetOperator meetOp; // meet operator for preds or succ

        BBVal in; // in[B]
        BBVal out; // out[B]
        
        // gen and kill sets; Should be calculated by the specific analysis and passed to DFF
        BBVal gen;
        BBVal kill;

        BitVector T; // Top value of the semi lattice
        BitVector B; // Bottom value of the semi lattice

        BitVector (*transferFunc)(BitVector, BitVector, BitVector); // function pointer to the transfer function of the analysis class

        BitVector applyMeet(BitVector b1, BitVector b2); //function to apply meet 

        // function to generate possible return blocks
        BBList getPossibleExitBlocks();

        public:
        // constructors for DFF
        DFF();
        DFF(Function *F, bool direction, meetOperator meetOp, unsigned bitvec_size, transferFuncTy transferFunc, bool boundary_val);

        // methods to set specific sets
        void setGen(BBVal gen);
        void setKill(BBVal kill);
        void setBoundary(bool direction, bool boundary_val, unsigned bitvec_size);

        // function to print all results on convergence
        template<class A> 
        void printRes(std::map<A, unsigned> mapping);

        template<class A> 
        void print(BitVector b, A rev_mapping[]);

        // destructor for DFF
        ~DFF();

        void runAnalysis(); // traversal of basicblocks based on the direction boolean

    };


// print functions moved here. C++ requirement
  template<class A> 
  void DFF::printRes(std::map<A, unsigned> mapping) {

    A rev_mapping[mapping.size()];

    for (auto ele : mapping) {

      unsigned ind = ele.second;
      A val = ele.first;

      rev_mapping[ind] = val;

    }

    for (BasicBlock &B: *F) {

      StringRef bName = B.getName();

      outs () << "==============" + bName + "==============" << "\n";

      outs () << "\nIN: \n";
      print<A>(in[&B], rev_mapping);

      outs () << "\nGEN: \n";
      print<A>(gen[&B], rev_mapping);

      outs () << "\nKILL: \n";
      print<A>(kill[&B], rev_mapping);

      outs () << "\nOUT: \n";
      print<A>(out[&B], rev_mapping);

      outs () << "\n====================================" << "\n";

    }

  }

  template<class A> 
  void DFF::print(BitVector b, A rev_mapping[]) {

    for (int i=0; i<b.size(); i++) {

      if (b[i]) {

        //outs() << rev_mapping[i]->dump() << "\n";
        rev_mapping[i]->dump();

      }
    }
  }
}

#endif
