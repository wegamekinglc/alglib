#include "costcalculator.hpp"
#include <assert.h>

// cpu calculation

CostCalculator::CostCalculator(const real_1d_array expectReturn,
                        const real_2d_array& varMatrix,
                        const real_1d_array& tradingCost,
                        const real_1d_array& currentWeight)
{
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());
    assert(tradingCost.length() == varMatrix.rows());
    assert(currentWeight.length() == varMatrix.rows());
    variableNumber_ = expectReturn.length();

    xReal_.resize(variableNumber_, 1);
    varMatrix_.resize(variableNumber_, variableNumber_);
    expectReturn_.resize(variableNumber_);
    tradingCost_.resize(variableNumber_);
    currentWeight_.resize(variableNumber_);

    for (int i = 0; i != variableNumber_; ++i)
    {
        expectReturn_(i) = expectReturn[i];
        tradingCost_(i) = tradingCost[i];
        currentWeight_(i) = currentWeight[i];
        for (int j = 0; j != variableNumber_; ++j)
            varMatrix_(i, j) = varMatrix[i][j];
    }
}


void CostCalculator::calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad)
{

    for (int i = 0; i != variableNumber_; ++i)
        xReal_(i) = xWeight[i];

    // risk grad
    VectorXd riskGrad = varMatrix_ * xReal_;

    // weight change
    VectorXd weightChange = xReal_ - currentWeight_;

    func = 0.5 * xReal_.dot(riskGrad) + weightChange.cwiseAbs().dot(tradingCost_) - expectReturn_.dot(xReal_);
    VectorXd tmp = riskGrad + weightChange.cwiseSign().cwiseProduct(tradingCost_) - expectReturn_;

    for (int i = 0; i != variableNumber_; ++i)
        grad(i) = tmp(i);
}


void calculate(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr)
{
    ((CostCalculator*)ptr)->calculateCost(xWeight, func, grad);
}

// cuda stuff

#define IDX2C(i, j, ld) (((j)*(ld)) + (i))


CostCalculator_cuda::CostCalculator_cuda(const real_1d_array expectReturn, const real_2d_array &varMatrix,
    const real_1d_array &tradingCost, const real_1d_array &currentWeight)
    : currentWeight_(currentWeight), tradingCost_(tradingCost) {
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());
    assert(tradingCost.length() == varMatrix.rows());
    assert(currentWeight.length() == varMatrix.rows());
    variableNumber_ = expectReturn.length();

    cudaMalloc((void **)&varMatrix_, variableNumber_ * variableNumber_ * sizeof(value_type));
    cudaMalloc((void **)&expectReturn_, variableNumber_ * sizeof(value_type));
    cudaMalloc((void **)&xReal_, variableNumber_ * sizeof(value_type));
    cudaMalloc((void **)&y_, variableNumber_ * sizeof(value_type));
    tmp_ = new value_type[variableNumber_];

    value_type *tmpMatrix = new value_type[variableNumber_ * variableNumber_];
    exp_host = new value_type[variableNumber_];
    for (int i = 0; i != variableNumber_; ++i) {
        exp_host[i] = expectReturn[i];
        for (int j = 0; j != variableNumber_; ++j)
            tmpMatrix[IDX2C(i, j, variableNumber_)] = varMatrix[i][j];
    }

    cudaMemcpy(varMatrix_, &tmpMatrix[0], sizeof(value_type) * variableNumber_ * variableNumber_,
        cudaMemcpyHostToDevice);
    cudaMemcpy(expectReturn_, &exp_host[0], sizeof(value_type) * variableNumber_, cudaMemcpyHostToDevice);

    delete[] tmpMatrix;
}

void CostCalculator_cuda::calculateCost(const real_1d_array &xWeight, double &func, real_1d_array &grad) {

    for (int i = 0; i < variableNumber_; ++i) {
        tmp_[i] = xWeight[i];
    }

    cudaMemcpy(xReal_, &tmp_[0], sizeof(value_type) * variableNumber_, cudaMemcpyHostToDevice);

    cublasHandle_t handle;
    cublasCreate(&handle);

    //
    value_type returnContribution;
#ifndef _USEDOUBLE
    cublasSdot(handle, variableNumber_, xReal_, 1, expectReturn_, 1, &returnContribution);
#else
    cublasDdot(handle, variableNumber_, xReal_, 1, expectReturn_, 1, &returnContribution);
#endif

    const value_type alpha = 1.0;
    const value_type beta = 0.0;
    value_type totalRiskCost = 0.0;
#ifndef _USEDOUBLE
    cublasSsymv(handle, CUBLAS_FILL_MODE_LOWER, variableNumber_, &alpha, varMatrix_, variableNumber_, xReal_, 1, &beta, y_, 1);
    cublasSdot(handle, variableNumber_, xReal_, 1, y_, 1, &totalRiskCost);
#else
    cublasDsymv(handle, CUBLAS_FILL_MODE_LOWER, variableNumber_, &alpha, varMatrix_, variableNumber_, xReal_, 1, &beta, y_, 1);
    cublasDdot(handle, variableNumber_, xReal_, 1, y_, 1, &totalRiskCost);
#endif

    cudaMemcpy(tmp_, y_, sizeof(value_type) * variableNumber_, cudaMemcpyDeviceToHost);

    //
    value_type totalTradingCost = 0.0;

    for (int i = 0; i < variableNumber_; ++i) {
        const value_type change = xWeight[i] - currentWeight_[i];
        if (change < 0) {
            totalTradingCost -= change * tradingCost_[i];
            grad[i] = tmp_[i] - exp_host[i] - tradingCost_[i];
        }
        else {
            totalTradingCost += change * tradingCost_[i];
            grad[i] = tmp_[i] - exp_host[i] + tradingCost_[i];
        }
    }

    func = 0.5 * totalRiskCost + totalTradingCost - returnContribution;
}

void calculate_cuda(const real_1d_array &xWeight, double &func, real_1d_array &grad, void *ptr) {
    ((CostCalculator_cuda *)ptr)->calculateCost(xWeight, func, grad);
}