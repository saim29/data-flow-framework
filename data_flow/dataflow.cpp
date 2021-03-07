
////////////////////////////////////////////////////////////////////////////////

#include "dataflow.h"

namespace llvm {

  // Add code for your dataflow abstraction here.


  BitVector DFF::set_op(BitVector b1, BitVector b2, meetOperator meetOp) {

    unsigned size = b1.size();
    BitVector u = BitVector(size);

    for (int i=0; i<size; i++) {

      if (meetOp == UNION) {

        u[i] = b1[i] || b2[i];

      }
      else if (meetOp == INTERSECTION) {

        u[i] = b1[i] && b2[i];

      }
    }

    return u;
  }

  void DFF::traverseCFG() {

    if (direction) {


    } else {

    }

  }
}
