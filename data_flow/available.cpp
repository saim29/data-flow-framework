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
  // declare transfer function here
  BitVector transfer_function(BitVector in, BitVector e_gen, BitVector e_kill) {

    //BitVector intermediate = set_diff(in, e_kill);
    //return set_union(intermediate, e_gen);

    return set_union(in, e_gen);

  }


  class AvailableExpressions : public FunctionPass {
    
  public:
    static char ID;
    
    AvailableExpressions() : FunctionPass(ID) { }

    
    virtual bool runOnFunction(Function& F) {
      
      // Here's some code to familarize you with the Expression
      // class and pretty printing code we've provided:

      // traverse basicblocks to find a mapping between bitvector indexes and variables
      map_indexes(F);

      // initialize top element and bottom element according to the meetOp
      unsigned size_bitvec = exp_bvec_mapping.size();

      //initialize data flow framework
      DFF dff(&F, false, INTERSECTION, size_bitvec, &transfer_function, false);

      // compute use and def sets here
      populate_egen_ekill(F);

      // pass the use and def sets to the DFF
      dff.setGen(e_gen);
      dff.setKill(e_kill);

      // pass everything to the dff and start the analysis
      dff.runAnalysis();

      // print the results
      dff.printRes<Expression*>(exp_bvec_mapping);
      // Did not modify the incoming Function.
      return false;
      


      /*vector<Expression> expressions;
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
      return false;*/
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

            Expression *exp = new Expression(&I);

            auto iter = exp_bvec_mapping.begin();
            while(iter != exp_bvec_mapping.end()) {
              // Check if the expression generated above and the one pointed by the iterator are == 
              // == seems to be defined in available-support and compares the operands and operator
              if(*(iter->first) == *exp) {
                // Found it. Already exists in the bitvector. delete exp and break out
                delete exp;
                break;
              }

              iter++;
            }

            // Not found. Add it to the bit vector and increment the index
            if(iter == exp_bvec_mapping.end()){
              exp_bvec_mapping.insert({exp, ind});
              ind++;
            } 
          }
        }
      }
    }


    // We have the U list of all expressions (with their indices)
    // Just generate and kill them per basic block
    void populate_egen_ekill(Function &F) {
    
      unsigned size = exp_bvec_mapping.size();
      for (BasicBlock &B: F) {
        
        BitVector bvec(size);
        e_gen.insert({&B, bvec});
        e_kill.insert({&B, bvec});

        for (Instruction &I: B) {

          // e_gen part
          // Convert to expression only if a binary operator 
          if(BinaryOperator *BO = dyn_cast<BinaryOperator>(&I)) {
            Expression exp = Expression(&I);

            // Now look this expression up
            auto iter =  exp_bvec_mapping.begin();
            while(iter != exp_bvec_mapping.end()) {
              // Check if the expression generated above and the one pointed by the iterator are == 
              // == seems to be defined in available-support and compares the operands and operator
              if(*(iter->first) == exp) {
                unsigned index = iter->second;
                e_gen[&B][index] = 1;
                break;
              }
                
              // Keep iterating
              iter++;
            }
          }
          

          //e_kill is unnecessary since you will never ever re-assign a variable in SSA. 





          

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
