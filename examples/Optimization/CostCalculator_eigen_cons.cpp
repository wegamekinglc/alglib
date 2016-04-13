#include "CostCalculator_eigen_cons.hpp"
#include <optimization.h>
#include "assert.h"

CostCalculator_eigen_cons::CostCalculator_eigen_cons(const real_1d_array expectReturn,
                                           const real_2d_array &varMatrix,
                                           const real_1d_array &tradingCost,
                                           const real_1d_array &currentWeight,
										   const double& lambda,
										   const double& target) 
										   :lambda_(lambda), target_(target) {
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

void CostCalculator_eigen_cons::calculateCost(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac)
{
	for (int i = 0; i < variableNumber_; ++i) xReal_(i) = xWeight[i];

	// risk grad
	VectorXd riskGrad = varMatrix_ * xReal_;

	// weight change
	VectorXd weightChange = xReal_ - currentWeight_;

	fi[0] = 0.5 * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
	fi[1] = (weightChange.cwiseAbs().dot(tradingCost_) - target_) / tradingCost_[0];

	VectorXd tmp = riskGrad - expectReturn_;

	for (int i = 0; i < variableNumber_; ++i)
	{
		jac[0][i] = tmp(i);
	}

	tmp = weightChange.cwiseSign().cwiseProduct(tradingCost_) / tradingCost_[0];

	for (int i = 0; i < variableNumber_; ++i)
	{
		jac[1][i] = tmp(i);
	}

}

void CostCalculator_eigen_cons::calculateCost(const real_1d_array &xWeight, double &func, real_1d_array &grad) {
	for (int i = 0; i < variableNumber_; ++i) xReal_(i) = xWeight[i];

	// risk grad
	VectorXd riskGrad = varMatrix_ * xReal_;

	// weight change
	VectorXd weightChange = xReal_ - currentWeight_;

	func = 0.5 * xReal_.dot(riskGrad) + lambda_ * weightChange.cwiseAbs().dot(tradingCost_) - expectReturn_.dot(xReal_);
	VectorXd tmp = riskGrad + lambda_ * weightChange.cwiseSign().cwiseProduct(tradingCost_) - expectReturn_;

	for (int i = 0; i < variableNumber_; ++i) grad(i) = tmp(i);
}

double CostCalculator_eigen_cons::costWithoutTrading(const real_1d_array& xWeight)
{
	for (int i = 0; i < variableNumber_; ++i) xReal_(i) = xWeight[i];

	// risk grad
	VectorXd riskGrad = varMatrix_ * xReal_;

	double func = 0.5 * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
	return func;

}

void calculate_eigen_cons(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr) {
	((CostCalculator_eigen_cons *)ptr)->calculateCost(xWeight, func, grad);
}

void calculate_eigen_cons(const real_1d_array& xWeight, real_1d_array &fi, real_2d_array &jac, void *ptr) {
	((CostCalculator_eigen_cons *)ptr)->calculateCost(xWeight, fi, jac);
}


TargetFunction::TargetFunction(const real_1d_array& expectReturn,
	const real_2d_array& varMatrix,
	const real_1d_array& tradingCost,
	const real_1d_array& currentWeight,
	double target)
	:tradingCost_(tradingCost), currentWeight_(currentWeight), targetWeight_(currentWeight), costCalc_(expectReturn, varMatrix, tradingCost, currentWeight, 1.0, target), target_(target)
{
	variableNumber_ = varMatrix.rows();
}

double TargetFunction::operator()(double loglambda) const
{
	costCalc_.setLambda(std::exp(loglambda));

	// bounded constraints
	real_1d_array bndl;
	bndl.setlength(variableNumber_);
	for (int i = 0; i != variableNumber_; ++i)
		bndl[i] = 0.0;

	real_1d_array bndu;
	bndu.setlength(variableNumber_);
	for (int i = 0; i != variableNumber_; ++i)
		bndu[i] = 1.0;

	// linear constraints
	real_2d_array conMatrix;
	integer_1d_array condType = "[0]";
	conMatrix.setlength(1, variableNumber_ + 1);
	for (int i = 0; i != variableNumber_; ++i)
		conMatrix[0][i] = 1.0;
	conMatrix[0][variableNumber_] = 1.0;

	// stop condition
	double epsg = 1e-8;
	double epsf = 1e-8;
	double epsx = 1e-8;
	alglib::ae_int_t maxits = 0;

	real_1d_array guess;
	guess.setlength(variableNumber_);
	for (int i = 0; i != variableNumber_; ++i)
		guess[i] = 1.0 / variableNumber_;

	state_analytic_ = alglib::minbleicstate();
	rep_analytic_ = alglib::minbleicreport();

	alglib::minbleiccreate(guess, state_analytic_);
	alglib::minbleicsetlc(state_analytic_, conMatrix, condType);
	alglib::minbleicsetbc(state_analytic_, bndl, bndu);
	alglib::minbleicsetcond(state_analytic_, epsg, epsf, epsx, maxits);

	alglib::minbleicoptimize(state_analytic_, calculate_eigen_cons, NULL, &costCalc_);
	alglib::minbleicresults(state_analytic_, targetWeight_, rep_analytic_);

	double totoalTradingCost = 0.0;

	for (int i = 0; i < variableNumber_; ++i)
	{
		double change = targetWeight_[i] - currentWeight_[i];
		if (change < 0.0)
			totoalTradingCost -= change * tradingCost_[i];
		else
			totoalTradingCost += change * tradingCost_[i];
	}
	return totoalTradingCost - target_;
}

double TargetFunction::cost() const
{
	return costCalc_.costWithoutTrading(targetWeight_);
}
