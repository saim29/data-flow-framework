// ECE 5984 S21 Assignment 2: liveness.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"

using namespace llvm;

namespace {

  // declare transfer function here
  BitVector transfer_function(BitVector out, BitVector use, BitVector def) {

    BitVector intermediate = set_diff(out, def);
    return set_union(intermediate, use);

  }

  class Liveness : public FunctionPass {

  public:
    static char ID;
    Liveness() : FunctionPass(ID) { }

    

    virtual bool runOnFunction(Function& F) {

      // traverse basicblocks to find a mapping between bitvector indexes and variables
      map_indexes(F);

      // initialize top element and bottom element according to the meetOp
      unsigned size_bitvec = bvec_mapping.size();

      //initialize data flow framework
      DFF dff(&F, true, UNION,  size_bitvec, &transfer_function, false);

      // compute use and def sets here
      populate_use_and_def(F);

      // pass the use and def sets to the DFF
      dff.setGen(use);
      dff.setKill(def);

      //dff.printRes<Value*>(bvec_mapping);


      // pass everything to the dff and start the analysis
      dff.runAnalysis();

      // print the results
      dff.printRes<Value*>(bvec_mapping);

      // Did not modify the incoming Function.
      return false;
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }

    void map_indexes(Function &F) {

      unsigned ind = 0;
      for (BasicBlock &B: F) {

        for (Instruction &I: B) {

          Value *v = &I;
          if (v->getNumUses() > 0) {

            if (bvec_mapping.find(v) == bvec_mapping.end()) {

              //v->dump();
              bvec_mapping.insert({v, ind});
              ind++;
            }
          }
        }
      }
    }

    void populate_use_and_def(Function &F) {

      unsigned size = bvec_mapping.size();
      for (BasicBlock &B: F) {
        
        BitVector bvec(size, false);
        use.insert({&B, bvec});
        def.insert({&B, bvec});

        for (Instruction &I: B) {

          Value *v = &I;

          if (bvec_mapping.find(v) != bvec_mapping.end()) {

            unsigned ind = bvec_mapping[v];

            if (!use[&B][ind])
              def[&B][ind] = 1;

          }

          // ignore phi node operands since they are pseudo-instructions?
          for (User::op_iterator op = I.op_begin(), opE = I.op_end(); op != opE; ++op) {

              Value* val = *op;

              if (bvec_mapping.find(val) != bvec_mapping.end()) {
                
                unsigned ind = bvec_mapping[val];

                if (!def[&B][ind]) {
                  use[&B][ind] = 1; 
                }

              }
          }

        }
      }

    }

  private:

    VMap bvec_mapping; // maps the domain to the indexes in the bitmap
    BBVal use; // use set for all basic blocks
    BBVal def; // def set for all basic blocks

  };

  char Liveness::ID = 0;
  RegisterPass<Liveness> X("liveness", "ECE 5984 Liveness");
}
