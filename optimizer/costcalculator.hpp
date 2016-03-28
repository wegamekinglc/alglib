#ifndef ALGLIB_OPTIMIZER_COSTCALCULATOR_ANALYTIC_HPP
#define ALGLIB_OPTIMIZER_COSTCALCULATOR_ANALYTIC_HPP

#include "types.hpp"
#include <cuda_runtime.h>
#include <cublas_v2.h>

#ifdef USE_MKL_LIB
#define EIGEN_USE_MKL_ALL
#endif

#ifndef _USEDOUBLE
typedef float value_type;
#else
typedef double value_type;
#endif

#include "types.hpp"
#include <Eigen/Eigen>

using Eigen::MatrixXd;
using Eigen::VectorXd;


class CostCalculator {
public:
    CostCalculator(const real_1d_array expectReturn,
        const real_2d_array& varMatrix,
        const real_1d_array& tradingCost,
        const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad);

private:
    VectorXd expectReturn_;
    MatrixXd varMatrix_;
    VectorXd tradingCost_;
    VectorXd currentWeight_;
    int variableNumber_;
    VectorXd xReal_;
};


void calculate(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);


class CostCalculator_cuda {
public:
    CostCalculator_cuda(const real_1d_array expectReturn,
        const real_2d_array& varMatrix,
        const real_1d_array& tradingCost,
        const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad);

    ~CostCalculator_cuda()
    {
        cudaFree(expectReturn_);
        cudaFree(varMatrix_);
        cudaFree(xReal_);
        cudaFree(y_);
        delete[] tmp_;
        delete[] exp_host;
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


void calculate_cuda(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);

#endif
