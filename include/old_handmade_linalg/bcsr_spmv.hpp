#ifndef _YAFEL_BCSR_SPMV_HPP
#define _YAFEL_BCSR_SPMV_HPP

/*
 * Sparse matrix-vector multiplication subroutine for a 
 * block-compressed-sparse-row matrix. This will only be
 * implemented for Vector<dataType> rather than VectorExpression<dataType>
 * in order to force conversion to take advantage of known
 * data reuse.
 */


#include "yafel_globals.hpp"
#include "Vector.hpp"
#include "sparse_bcsr.hpp"

#include <cassert>
#include <cstdlib>
#include <iostream>

YAFEL_NAMESPACE_OPEN

template<unsigned BLOCK, typename dataType>
Vector<dataType> bcsr_spmv(const sparse_bcsr<BLOCK, dataType> &A,
                           const Vector<dataType> &x)
{

#ifndef _OPTIMIZED
  assert(A.cols() == x.size() && "Error: bcsr_spmv dimension mismatch");
#endif

  Vector<dataType> b(A.rows(), 0);
  std::size_t BLOCK2 = BLOCK*BLOCK;
  
  for(std::size_t brow=0; brow<A.rows()/BLOCK; ++brow) {

    std::size_t idxmin, idxmax;
    idxmin = A.brow_ptr[brow];
    idxmax = A.brow_ptr[brow+1];
    
    //accumulate
    dataType accum[BLOCK] = { };
    for(std::size_t i=0; i<BLOCK; ++i) {
      accum[i] = dataType(0);
    }
    
    for(std::size_t idx=idxmin; idx<idxmax; ++idx) {
      for(std::size_t i=0; i<BLOCK; ++i) {
        for(std::size_t j=0; j<BLOCK; ++j) {
          std::size_t col = A.bcol_index[idx]*BLOCK + j;
          accum[i] += A.data[idx*BLOCK2 + i*BLOCK + j]*x(col);
        }
      }
    }
    for(std::size_t i=0; i<BLOCK; ++i) {
      b(BLOCK*brow + i) = accum[i];
    }
    
  }

  return b;
}



YAFEL_NAMESPACE_CLOSE

#endif
