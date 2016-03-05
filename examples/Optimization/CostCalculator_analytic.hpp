#ifndef ALGLIB_COSTCALCULATOR_ANALYTIC_HPP
#define ALGLIB_COSTCALCULATOR_ANALYTIC_HPP

#include <assert.h>
#include "types.hpp"

class CostCalculator_analytic {
    public:
        CostCalculator_analytic(const real_1d_array expectReturn,
            const real_2d_array& varMatrix,
            const real_1d_array& tradingCost,
            const real_1d_array& currentWeight);

        void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad) const;

    private:
        real_1d_array expectReturn_;
        real_2d_array varMatrix_;
        real_1d_array tradingCost_;
        real_1d_array currentWeight_;
        int variableNumber_;
};


void calculate_analytic (const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);

#endif
