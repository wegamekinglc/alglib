//
// Created by wegamekinglc on 2016/4/12.
//

#ifndef ALGLIB_OPTIMIZATION_COSTCALCULATOR_EIGEN_CONS_HPP
#define ALGLIB_OPTIMIZATION_COSTCALCULATOR_EIGEN_CONS_HPP

#ifdef USE_MKL_LIB
#define EIGEN_USE_MKL_ALL
#endif

#include "types.hpp"
#include <Eigen/Eigen>

using Eigen::MatrixXd;
using Eigen::VectorXd;


class CostCalculator_eigen_cons {
public:
    CostCalculator_eigen_cons(const real_1d_array expectReturn,
                              const real_2d_array& varMatrix,
                              const real_1d_array& tradingCost,
                              const real_1d_array& currentWeight);

    void calculateCost(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac);

private:
    VectorXd expectReturn_;
    MatrixXd varMatrix_;
    VectorXd tradingCost_;
    VectorXd currentWeight_;
    int variableNumber_;
    VectorXd xReal_;
};


void calculate_eigen_cons(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac, void *ptr);

#endif //ALGLIB_COSTCALCULATOR_EIGEN_CONS_HPP
