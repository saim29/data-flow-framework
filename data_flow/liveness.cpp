// ECE 5984 S21 Assignment 2: liveness.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"

using namespace llvm;

namespace {



  class Liveness : public FunctionPass {

  public:
    static char ID;

    // Need to fill this out
    static BitVector livenessTransferFunc(BitVector input) {
      BitVector dummy;
      return dummy;
    };

    Liveness() : FunctionPass(ID) { 

      // Initialize the dataflow object

      BitVector (*livenessTransferFunc_P) (BitVector) = &livenessTransferFunc; 
      dataFlow dataFlowObj(BACKWARD, UNION, livenessTransferFunc_P);


    }

    virtual bool runOnFunction(Function& F) {

      // Did not modify the incoming Function.




      return false;
    }

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }

  private:
  };

  char Liveness::ID = 0;
  RegisterPass<Liveness> X("liveness", "ECE 5984 Liveness");
}
