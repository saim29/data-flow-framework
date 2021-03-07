
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


/*

Implementation Notes:
- Value numbering to setup bitvectors

*/

namespace llvm {

    // Direction
    typedef enum Direction {
        FORWARD,
        BACKWARD
    } Direction;

    // MeetOperator
    typedef enum MeetOperator {
        UNION,
        INTERSECTION
    } MeetOperator;

    // Transfer Function  
    

    class dataFlow  : FunctionPass {

    private:
        Direction direction;
        MeetOperator meetOp;
        BitVector (*transferFunc) (BitVector in);
        BitVector (*domainCreator)();
        
        
        // Mapping of the IN and OUT bitvectors to the basic block

        ValueMap<BasicBlock*, BitVector> in; //in[B]
        ValueMap<BasicBlock*, BitVector> out; //out[B]



        BitVector T; //Top value of the semi lattice
        BitVector B; //Bottom value of the semi lattice
        
    public:

        static char ID;

        dataFlow(Direction directionInput, MeetOperator meetOperatorInput, BitVector (*transferFuncInput) (BitVector)) : FunctionPass(ID) {

            direction = directionInput;
            meetOp = meetOperatorInput;
            transferFunc = transferFuncInput;
            //domainCreator = domainCreatorInput;
        };


        

        bool runOnFunction(Function &F)  override {
            
            // Populate basic block in and out maps


            for(BasicBlock& BB : F) {


            }
            return false;
        }




        
    

    virtual void getAnalysisUsage(AnalysisUsage& AU) const {
      AU.setPreservesAll();
    }      
        

        

    };

    

}

#endif
