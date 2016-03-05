#include "CostCalculator_adept.hpp"

CostCalculator_adept::CostCalculator_adept(const real_1d_array expectReturn,
                     const real_2d_array& varMatrix,
                     const real_1d_array& tradingCost,
                     const real_1d_array& currentWeight)
{
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());
    assert(tradingCost.length() == varMatrix.rows());
    assert(currentWeight.length() == varMatrix.rows());
    variableNumber_ = expectReturn.length();

    expectReturn_ = boost::numeric::ublas::vector<double>(variableNumber_);
    tradingCost_ = boost::numeric::ublas::vector<double>(variableNumber_);
    currentWeight_ = boost::numeric::ublas::vector<double>(variableNumber_);
    varMatrix_ = boost::numeric::ublas::matrix<double>(variableNumber_, variableNumber_);

    for (int i = 0; i != variableNumber_; ++i) {
        expectReturn_[i] = expectReturn[i];
        tradingCost_[i] = tradingCost[i];
        currentWeight_[i] = currentWeight[i];
        for (int j = 0; j != variableNumber_; ++j)
            varMatrix_(i, j) = varMatrix[i][j];
    }
}

void CostCalculator_adept::calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad)
{

    using adept::adouble;

    boost::numeric::ublas::vector<adouble> realX(variableNumber_);

    adept::set_values(&realX[0], variableNumber_, &xWeight[0]);

    stack_.new_recording();

    adouble totalCost = 0.0;

    totalCost -= static_cast<adouble>(boost::numeric::ublas::inner_prod(expectReturn_, realX));

    totalCost += 0.5 * static_cast<adouble>(boost::numeric::ublas::inner_prod(
            boost::numeric::ublas::prod(varMatrix_, realX), realX));

    boost::numeric::ublas::vector<adouble> weightChanges = realX - currentWeight_;

    // trading cost
    for (int i = 0; i != variableNumber_; ++i) {
        if (weightChanges[i] < 0.)
            totalCost += -weightChanges[i] * tradingCost_[i];
        else
            totalCost += weightChanges[i] * tradingCost_[i];
    }
    totalCost.set_gradient(1.0);
    stack_.compute_adjoint();
    adept::get_gradients(&realX[0], variableNumber_, &grad[0]);
    func = totalCost.value();

}


void calculate_adept(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr)
{
    ((CostCalculator_adept*)ptr)->calculateCost(xWeight, func, grad);
}




