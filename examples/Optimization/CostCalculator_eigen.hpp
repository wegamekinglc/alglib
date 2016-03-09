#ifndef ALGLIB_OPTIMIZATION_COSTCALCULATOR_EIGEN_HPP
#define ALGLIB_OPTIMIZATION_COSTCALCULATOR_EIGEN_HPP

#ifdef USE_MKL_LIB
#define EIGEN_USE_MKL_ALL
#endif

#include "types.hpp"
#include <Eigen/Eigen>

using Eigen::MatrixXd;
using Eigen::VectorXd;


class CostCalculator_eigen {
public:
    CostCalculator_eigen(const real_1d_array expectReturn,
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


void calculate_eigen (const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);

#endif //ALGLIB_COSTCALCULATOR_EIGEN_HPP
