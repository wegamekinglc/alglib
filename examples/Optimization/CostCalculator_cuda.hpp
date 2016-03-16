//
// Created by root on 16-3-16.
//

#ifndef ALGLIB_OPTIMIZATION_COSTCALCULATOR_CUDA_HPP
#define ALGLIB_OPTIMIZATION_COSTCALCULATOR_CUDA_HPP

#include "types.hpp"
#include "cuda_runtime.h"
#include "cublas_v2.h"

class CostCalculator_cuda {
public:

    typedef float value_type;

    CostCalculator_cuda(const real_1d_array expectReturn,
                        const real_2d_array& varMatrix,
                        const real_1d_array& tradingCost,
                        const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad);

    ~CostCalculator_cuda()
    {
        cudaFree(expectReturn_);
        cudaFree(varMatrix_);
        cudaFree(tradingCost_);
        delete [] currentWeight_;
        delete [] xReal_;
    }

private:
    value_type* expectReturn_;
    value_type* varMatrix_;
    value_type* tradingCost_;
    value_type* currentWeight_;
    int variableNumber_;
    value_type* xReal_;
};


void calculate_analytic (const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);

#endif //ALGLIB_COSTCALCULATOR_CUDA_HPP
