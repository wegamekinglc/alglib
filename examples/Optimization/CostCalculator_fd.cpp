//
// Created by root on 16-3-5.
//

#include "CostCalculator_fd.hpp"
#include <linalg.h>


CostCalculator_fd::CostCalculator_fd(const real_1d_array expectReturn,
                  const real_2d_array& varMatrix,
                  const real_1d_array& tradingCost,
                  const real_1d_array& currentWeight)
        :expectReturn_(expectReturn), varMatrix_(varMatrix), tradingCost_(tradingCost), currentWeight_(currentWeight)
{
    assert(expectReturn_.length() == varMatrix_.rows());
    assert(varMatrix_.rows() == varMatrix_.cols());
    assert(tradingCost_.length() == varMatrix_.rows());
    assert(currentWeight_.length() == varMatrix_.rows());
    variableNumber_ = expectReturn_.length();
}


void CostCalculator_fd::calculateCost(const real_1d_array& xWeight, double& func) const
{
    real_1d_array xReal = xWeight;

    // 计算 expect return contribution
    double returnContribution = alglib::vdotproduct(&xReal[0], &expectReturn_[0], variableNumber_);

    // 计算 risk cost
    real_1d_array yVector;
    yVector.setlength(variableNumber_);
    alglib::rmatrixmv(variableNumber_,
                      variableNumber_,
                      varMatrix_,
                      0,
                      0,
                      0,
                      xReal,
                      0,
                      yVector,
                      0);

    double totalRiskCost = 0.5 * alglib::vdotproduct(&yVector[0], 1, &xReal[0], 1, variableNumber_);

    // 计算 trading cost
    double totalTradingCost = 0.;
    alglib::vsub(&xReal[0], &currentWeight_[0], variableNumber_);
    for (int i = 0; i != variableNumber_; ++i) {
        if (xReal[i] < 0.)
            totalTradingCost += -xReal[i] * tradingCost_[i];
        else
            totalTradingCost += xReal[i] * tradingCost_[i];
    }

    // 整体 cost
    func = totalRiskCost + totalTradingCost - returnContribution;
}


void calculate_fd(const real_1d_array& xWeight, double& func, void *ptr)
{
    ((CostCalculator_fd*)ptr)->calculateCost(xWeight, func);
}

