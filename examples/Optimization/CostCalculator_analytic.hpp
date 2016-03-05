#ifndef cost_calculator_analytic_hpp
#define cost_calculator_analytic_hpp

#include <ap.h>
#include <assert.h>

using alglib::real_1d_array;
using alglib::real_2d_array;
using alglib::integer_1d_array;

class CostCalculator_analytic {
    public:
        CostCalculator_analytic(const real_1d_array expectReturn,
            const real_2d_array& varMatrix,
            const real_1d_array& tradingCost,
            const real_1d_array& currentWeight)
            : expectReturn_(expectReturn), varMatrix_(varMatrix), tradingCost_(tradingCost), currentWeight_(currentWeight) {
            assert(expectReturn_.length() == varMatrix_.rows());
            assert(varMatrix_.rows() == varMatrix_.cols());
            assert(tradingCost_.length() == varMatrix_.rows());
            assert(currentWeight_.length() == varMatrix_.rows());
            variableNumber_ = expectReturn_.length();
        }

        void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad) const {

            // 计算 expect return contribution
            double returnContribution = 0.;
            for (int i = 0; i != variableNumber_; ++i)
                returnContribution += xWeight[i] * expectReturn_[i];

            // 计算 risk cost
            real_1d_array yVector;
            yVector.setlength(variableNumber_);
            alglib::rmatrixmv(variableNumber_,
                variableNumber_,
                varMatrix_,
                0,
                0,
                0,
                xWeight,
                0,
                yVector,
                0);

            double totalRiskCost = 0.5 * alglib::vdotproduct(&yVector[0], 1, &xWeight[0], 1, variableNumber_);

            // 计算 trading cost
            double totalTradingCost = 0.;
            for (int i = 0; i != variableNumber_; ++i) {
                double weightChange = xWeight[i] - currentWeight_[i];
                if (weightChange < 0.)
                    totalTradingCost += -weightChange * tradingCost_[i];
                else
                    totalTradingCost += weightChange * tradingCost_[i];
            }

            // 整体 cost
            func = totalRiskCost + totalTradingCost - returnContribution;

            /*
                计算 gradient
            */

            real_1d_array tradingGrad;
            tradingGrad.setlength(variableNumber_);
            for (int i = 0; i != variableNumber_; ++i) {
                double weightChange = xWeight[i] - currentWeight_[i];
                if (weightChange < 0.)
                    tradingGrad[i] = -tradingCost_[i];
                else
                    tradingGrad[i] = tradingCost_[i];
            }

            for (int i = 0; i != variableNumber_; ++i)
                grad[i] = yVector[i] - expectReturn_[i] + tradingGrad[i];
        }

    private:
        real_1d_array expectReturn_;
        real_2d_array varMatrix_;
        real_1d_array tradingCost_;
        real_1d_array currentWeight_;
        int variableNumber_;
};


inline void calculate_analytic (const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr) {
    ((CostCalculator_analytic*)ptr)->calculateCost(xWeight, func, grad);
}

#endif
