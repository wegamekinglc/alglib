#include "CostCalculator_eigen.hpp"
#include "assert.h"

CostCalculator_eigen::CostCalculator_eigen(const real_1d_array expectReturn,
                                           const real_2d_array &varMatrix,
                                           const real_1d_array &tradingCost,
                                           const real_1d_array &currentWeight) {
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

    for (int i = 0; i != variableNumber_; ++i) {
        expectReturn_(i) = expectReturn[i];
        tradingCost_(i) = tradingCost[i];
        currentWeight_(i) = currentWeight[i];
        for (int j = 0; j != variableNumber_; ++j)
            varMatrix_(i, j) = varMatrix[i][j];
    }
}


void CostCalculator_eigen::calculateCost(const real_1d_array &xWeight, double &func, real_1d_array &grad) {
    for (int i = 0; i < variableNumber_; ++i) xReal_(i) = xWeight[i];

    // risk grad
    VectorXd riskGrad = varMatrix_ * xReal_;

    // weight change
    VectorXd weightChange = xReal_ - currentWeight_;

    func = 0.5 * xReal_.dot(riskGrad) + weightChange.cwiseAbs().dot(tradingCost_) - expectReturn_.dot(xReal_);
    VectorXd tmp = riskGrad + weightChange.cwiseSign().cwiseProduct(tradingCost_) - expectReturn_;

    for (int i = 0; i < variableNumber_; ++i) grad(i) = tmp(i);
}


void calculate_eigen(const real_1d_array &xWeight, double &func, real_1d_array &grad, void *ptr) {
    ((CostCalculator_eigen *) ptr)->calculateCost(xWeight, func, grad);
}
