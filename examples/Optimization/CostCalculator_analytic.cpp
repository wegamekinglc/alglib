#include "CostCalculator_analytic.hpp"
#include <linalg.h>
#include <assert.h>

CostCalculator_analytic::CostCalculator_analytic(const real_1d_array expectReturn,
                                                 const real_2d_array &varMatrix,
                                                 const real_1d_array &tradingCost,
                                                 const real_1d_array &currentWeight)
        : expectReturn_(expectReturn), varMatrix_(varMatrix), tradingCost_(tradingCost), currentWeight_(currentWeight) {
    assert(expectReturn_.length() == varMatrix_.rows());
    assert(varMatrix_.rows() == varMatrix_.cols());
    assert(tradingCost_.length() == varMatrix_.rows());
    assert(currentWeight_.length() == varMatrix_.rows());
    variableNumber_ = expectReturn_.length();
    xReal_.setlength(variableNumber_);
}


void CostCalculator_analytic::calculateCost(const real_1d_array &xWeight, double &func, real_1d_array &grad) {

    alglib::vmove(&xReal_[0], &xWeight[0], variableNumber_);

    // expect return contribution
    double returnContribution = alglib::vdotproduct(&xReal_[0], &expectReturn_[0], variableNumber_);

    // risk cost
    real_1d_array yVector;
    yVector.setlength(variableNumber_);
    alglib::rmatrixmv(variableNumber_,
                      variableNumber_,
                      varMatrix_,
                      0,
                      0,
                      0,
                      xReal_,
                      0,
                      yVector,
                      0);

    double totalRiskCost = 0.5 * alglib::vdotproduct(&yVector[0], 1, &xReal_[0], 1, variableNumber_);

    // trading cost
    double totalTradingCost = 0.;
    alglib::vsub(&xReal_[0], &currentWeight_[0], variableNumber_);

    real_1d_array tradingGrad;
    tradingGrad.setlength(variableNumber_);

    for (int i = 0; i < variableNumber_; ++i) {
        if (xReal_[i] < 0.) {
            totalTradingCost += -xReal_[i] * tradingCost_[i];
            tradingGrad[i] = -tradingCost_[i];
        }
        else {
            totalTradingCost += xReal_[i] * tradingCost_[i];
            tradingGrad[i] = tradingCost_[i];
        }
    }

    // total cost
    func = totalRiskCost + totalTradingCost - returnContribution;

    // gradient
    for (int i = 0; i < variableNumber_; ++i)
        grad[i] = yVector[i] - expectReturn_[i] + tradingGrad[i];
}


void calculate_analytic(const real_1d_array &xWeight, double &func, real_1d_array &grad, void *ptr) {
    ((CostCalculator_analytic *) ptr)->calculateCost(xWeight, func, grad);
}
