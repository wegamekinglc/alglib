#ifndef ALGLIB_COSTCALCULATOR_ADEPT_HPP
#define ALGLIB_COSTCALCULATOR_ADEPT_HPP

#include "types.hpp"
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <adept.h>


class CostCalculator_adept {
public:
    CostCalculator_adept(const real_1d_array expectReturn,
                         const real_2d_array& varMatrix,
                         const real_1d_array& tradingCost,
                         const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad);

private:
    boost::numeric::ublas::vector<double> expectReturn_;
    boost::numeric::ublas::matrix<double> varMatrix_;
    boost::numeric::ublas::vector<double> tradingCost_;
    boost::numeric::ublas::vector<double> currentWeight_;
    int variableNumber_;
    adept::Stack stack_;
};


void calculate_adept(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);


#endif //ALGLIB_COSTCALCULATOR_ADEPT_HPP
