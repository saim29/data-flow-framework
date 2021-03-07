// ECE 5984 S21 Assignment 2: liveness.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"

using namespace llvm;

namespace {

  class LivenessFramework: public dataFlow {

    public:


      LivenessFramework() : dataFlow(BACKWARD, UNION) {}

      BitVector transferFunc(BitVector) override {
        BitVector dummy;
        return dummy;
      }


      void populateGlobalVector() override  {
        

      }

  };


  class Liveness : public FunctionPass {

  public:
    static char ID;
    LivenessFramework livenessFramework;
    

    Liveness() : FunctionPass(ID) { }

    

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
