//
// Created by root on 16-3-16.
//

#ifndef ALGLIB_OPTIMIZATION_COSTCALCULATOR_CUDA_HPP
#define ALGLIB_OPTIMIZATION_COSTCALCULATOR_CUDA_HPP

#include "types.hpp"
#include "cuda_runtime.h"
#include "cublas_v2.h"

#define _USEDOUBLE

class CostCalculator_cuda {
public:

#ifndef _USEDOUBLE
    typedef float value_type;
#else
    typedef double value_type;
#endif

    CostCalculator_cuda(const real_1d_array expectReturn,
                        const real_2d_array& varMatrix,
                        const real_1d_array& tradingCost,
                        const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, double& func,  real_1d_array& grad);

    ~CostCalculator_cuda()
    {
        cudaFree(expectReturn_);
        cudaFree(varMatrix_);
        cudaFree(xReal_);
        cudaFree(y_);
        delete [] tmp_;
        delete [] exp_host;
    }

private:
    value_type* expectReturn_;
    value_type* varMatrix_;
    real_1d_array tradingCost_;
    real_1d_array currentWeight_;
    int variableNumber_;
    value_type* xReal_;
    value_type* y_;
    value_type* tmp_;
    value_type* exp_host;
};


void calculate_cuda (const real_1d_array& xWeight, double& func,  real_1d_array& grad, void *ptr);

#endif //ALGLIB_COSTCALCULATOR_CUDA_HPP
