
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
    

    class dataFlow  {
        

    public:
        Direction direction;
        MeetOperator meetOp;

        dataFlow(Direction direction, MeetOperator meetOp) : direction(direction), meetOp (meetOp){ 
            
            outs() << "Here!" << direction << meetOp; 

        }

        virtual BitVector transferFunc (BitVector);
        virtual void populateGlobalVector();
        
    };
    

}

#endif
// Mapping of the IN and OUT bitvectors to the basic block

//        ValueMap<BasicBlock*, BitVector> in; //in[B]
//        ValueMap<BasicBlock*, BitVector> out; //out[B]



//        BitVector T; //Top value of the semi lattice
//        BitVector B; //Bottom value of the semi lattice
        