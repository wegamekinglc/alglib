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
#include <optimization.h>

using Eigen::MatrixXd;
using Eigen::VectorXd;


class CostCalculator_eigen_cons {
public:
    CostCalculator_eigen_cons(const real_1d_array expectReturn,
                              const real_2d_array& varMatrix,
                              const real_1d_array& tradingCost,
                              const real_1d_array& currentWeight,
							  const double& lambda,
							  const double& target);

	void calculateCost(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac);
	void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad);
	double costWithoutTrading(const real_1d_array& xWeight);
	void setLambda(const double& newLambda)
	{
		lambda_ = newLambda;
	}

private:
    VectorXd expectReturn_;
    MatrixXd varMatrix_;
    VectorXd tradingCost_;
    VectorXd currentWeight_;
    int variableNumber_;
    VectorXd xReal_;
	double lambda_;
	double target_;
};


void calculate_eigen_cons(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr);
void calculate_eigen_cons(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac, void *ptr);


class TargetFunction
{
public:
	TargetFunction(const real_1d_array& expectReturn,
		const real_2d_array& varMatrix,
		const real_1d_array& tradingCost,
		const real_1d_array& currentWeight,
		double target);
	double operator()(double loglambda) const;
	double cost() const;
	const real_1d_array& targetWeight() const { return targetWeight_;}
	const alglib::minbleicstate& state() const { return state_analytic_;}
	const alglib::minbleicreport& report() const { return rep_analytic_;}

private:
	const real_1d_array& tradingCost_;
	const real_1d_array& currentWeight_;
	mutable real_1d_array targetWeight_;
	mutable CostCalculator_eigen_cons costCalc_;
	mutable alglib::minbleicstate state_analytic_;
	mutable alglib::minbleicreport rep_analytic_;
	int variableNumber_;
	double target_;
};


#endif //ALGLIB_COSTCALCULATOR_EIGEN_CONS_HPP
