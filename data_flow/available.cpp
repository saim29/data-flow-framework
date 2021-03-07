// ECE 5984 S21 Assignment 2: available.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/IR/Constants.h"
#include "llvm/Support/raw_ostream.h"

#include "dataflow.h"
#include "available-support.h"

using namespace llvm;
using namespace std;

namespace {
  class AvailableExpressions : public FunctionPass {
    
  public:
    static char ID;
    
    AvailableExpressions() : FunctionPass(ID) { }

    
    virtual bool runOnFunction(Function& F) {
      
      // Here's some code to familarize you with the Expression
      // class and pretty printing code we've provided:
      
      vector<Expression> expressions;
      for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
	       BasicBlock* block = &*FI;
	       for (BasicBlock::iterator i = block->begin(), e = block->end(); i!=e; ++i) {
            
	         Instruction* I = &*i;
	         // We only care about available expressions for BinaryOperators
	         if (BinaryOperator *BI = dyn_cast<BinaryOperator>(I)) {
	           
	           expressions.push_back(Expression(BI));
	         }
	       }
      }
      
      // Print out the expressions used in the function
      outs() << "Expressions used by this function:\n";
      printSet(&expressions);
      
      // Did not modify the incoming Function.
      return false;
    }
    
    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }
    
    void map_indexes(Function &F) {

      // Use the expression class to get the indexes.
      unsigned ind = 0;

      for (BasicBlock &B: F) {

        for (Instruction &I: B) {
          
          // Convert to expression only if a binary operator 
          if(BinaryOperator *BO = dyn_cast<BinaryOperator>(&I)) {

            Expression exp = Expression(&I);

            EMap::iterator iter = exp_bvec_mapping.begin();
            while(iter != exp_bvec_mapping.end()) {
              if(iter->first == exp)
                break;

              iter++;
            }

            if(iter == exp_bvec_mapping.end()){
              exp_bvec_mapping.insert({exp, ind});
              ind++;
            } 
          }
        }
      }
    }

  
  
  private:
    EMap exp_bvec_mapping; // maps the domain to the indexes in the bitmap
    BBVal e_gen; // use set for all basic blocks
    BBVal e_kill; // def set for all basic blocks

  };
//  };
  
  char AvailableExpressions::ID = 0;
  RegisterPass<AvailableExpressions> X("available",
				       "ECE 5984 Available Expressions");
}
