#ifndef ALGLIB_COSTCALCULATOR_FD_HPP
#define ALGLIB_COSTCALCULATOR_FD_HPP

#include <assert.h>
#include "types.hpp"


class CostCalculator_fd {
    public:
        CostCalculator_fd(const real_1d_array expectReturn,
            const real_2d_array& varMatrix,
            const real_1d_array& tradingCost,
            const real_1d_array& currentWeight);

        void calculateCost(const real_1d_array& xWeight, double& func) const;
    private:
        real_1d_array expectReturn_;
        real_2d_array varMatrix_;
        real_1d_array tradingCost_;
        real_1d_array currentWeight_;
        int variableNumber_;
};


void calculate_fd(const real_1d_array& xWeight, double& func, void *ptr);


#endif 