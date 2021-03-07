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

      //initialize data flow framework
      DFF dff(true, INTERSECTION, &transfer_function);

      // compute use and def sets here

      // pass everything to the dff and start the analysis

      // Did not modify the incoming Function.
      return false;
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }

    void map_domain(Function &F) {

      unsigned ind = 0;
      for (BasicBlock &B: F) {

        for (Instruction &I: B) {

          Value *v = &I;
          if (v->getNumUses() > 0) {

            if (bvec_mapping.find(v) == bvec_mapping.end()) {
              bvec_mapping.insert({v, ind});
              ind++;
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
