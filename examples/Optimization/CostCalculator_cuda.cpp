//
// Created by root on 16-3-16.
//

#include "CostCalculator_cuda.hpp"
#include <assert.h>

CostCalculator_cuda::CostCalculator_cuda(const real_1d_array expectReturn, const real_2d_array &varMatrix,
                                         const real_1d_array &tradingCost, const real_1d_array &currentWeight)
{
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());
    assert(tradingCost.length() == varMatrix.rows());
    assert(currentWeight.length() == varMatrix.rows());
    variableNumber_ = expectReturn.length();


}
