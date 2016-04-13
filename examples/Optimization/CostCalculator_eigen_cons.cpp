#include "CostCalculator_eigen_cons.hpp"

#include "assert.h"

CostCalculator_eigen_cons::CostCalculator_eigen_cons(const real_1d_array expectReturn,
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


void CostCalculator_eigen_cons::calculateCost(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac) {
    for (int i = 0; i < variableNumber_; ++i) xReal_(i) = xWeight[i];

    // risk grad
    VectorXd riskGrad = varMatrix_ * xReal_;

    // weight change
    VectorXd weightChange = xReal_ - currentWeight_;

    fi[0] = 0.5 * xReal_.dot(riskGrad)  - expectReturn_.dot(xReal_);
	fi[1] = weightChange.cwiseAbs().dot(tradingCost_) - 10.0;

    VectorXd tmp = riskGrad - expectReturn_;

    for (int i = 0; i < variableNumber_; ++i)
    {
        jac[0][i] = tmp(i);
    }

    tmp = weightChange.cwiseSign().cwiseProduct(tradingCost_);

    for (int i = 0; i < variableNumber_; ++i)
    {
        jac[1][i] = tmp(i);
    }
}


void calculate_eigen_cons(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac, void *ptr) {
    ((CostCalculator_eigen_cons *) ptr)->calculateCost(xWeight, fi, jac);
}

