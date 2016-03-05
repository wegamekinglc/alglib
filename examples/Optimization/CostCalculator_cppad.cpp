#include "CostCalculator_cppad.hpp"

using CppAD::AD;


CostCalculator_cppad::CostCalculator_cppad(const real_1d_array expectReturn,
                     const real_2d_array& varMatrix,
                     const real_1d_array& tradingCost,
                     const real_1d_array& currentWeight)
{
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());
    assert(tradingCost.length() == varMatrix.rows());
    assert(currentWeight.length() == varMatrix.rows());
    variableNumber_ = expectReturn.length();

    expectReturn_.resize(variableNumber_);
    alglib::vmove(&expectReturn_[0], &expectReturn[0], variableNumber_);

    tradingCost_.resize(variableNumber_);
    alglib::vmove(&tradingCost_[0], &tradingCost[0], variableNumber_);

    currentWeight_.resize(variableNumber_);
    alglib::vmove(&currentWeight_[0], &currentWeight[0], variableNumber_);

    varMatrix_.resize(variableNumber_, variableNumber_);

    for (int i = 0; i != variableNumber_; ++i) {
        for (int j = 0; j != variableNumber_; ++j)
            varMatrix_(i,j) = varMatrix[i][j];
    }

    std::vector<AD<double> > X(variableNumber_);

    for (int i = 0; i != variableNumber_; ++i)
        X[i] = currentWeight_[i];

    CppAD::Independent(X);

    boost::numeric::ublas::vector<AD<double> > realX(variableNumber_);
    for (int i = 0; i != variableNumber_; ++i)
        realX[i] = X[i];

    AD<double> totalCost = 0.0;

    totalCost -= static_cast<AD<double> >(boost::numeric::ublas::inner_prod(expectReturn_, realX));

    totalCost += 0.5 * static_cast<AD<double> >(boost::numeric::ublas::inner_prod(
            boost::numeric::ublas::prod(varMatrix_, realX), realX));

    for (int i = 0; i != variableNumber_; ++i) {
        totalCost += CppAD::abs(realX[i] - currentWeight_[i]) * tradingCost_[i];
    }

    std::vector<AD<double> > Y(1);
    Y[0] = totalCost;
    fImpl_ = CppAD::ADFun<double>(X, Y);
}


void CostCalculator_cppad::calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad)
{

    std::vector<double> x(variableNumber_);
    alglib::vmove(&x[0], &xWeight[0], variableNumber_);

    std::vector<double> y = fImpl_.Forward(0, x);
    std::vector<double> jac = fImpl_.Jacobian(x);

    func = y[0];

    alglib::vmove(&grad[0], &jac[0], variableNumber_);
}


void calculate_cppad(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr)
{
    ((CostCalculator_cppad*)ptr)->calculateCost(xWeight, func, grad);
}
