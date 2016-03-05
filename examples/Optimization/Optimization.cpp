#include <iostream>

#include <boost/timer.hpp>
#include <boost/bind.hpp>
#include <optimization.h>
#include "CostCalculator_fd.hpp"
#include "CostCalculator_analytic.hpp"

#include <cppad/cppad.hpp>
#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>
#include <boost/numeric/ublas/blas.hpp>

#include "adept.h"

using CppAD::AD;

class CostCalculator_cppad {
    public:
        CostCalculator_cppad(const real_1d_array expectReturn,
            const real_2d_array& varMatrix,
            const real_1d_array& tradingCost,
            const real_1d_array& currentWeight) {
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

            totalCost -= boost::numeric::ublas::inner_prod(expectReturn_, realX);

            totalCost += 0.5 * boost::numeric::ublas::inner_prod(
                boost::numeric::ublas::prod(varMatrix_, realX), realX);

            for (int i = 0; i != variableNumber_; ++i) {
                AD<double> weightChange = realX[i] - currentWeight_[i];
                if (weightChange < 0.)
                    totalCost += -weightChange * tradingCost_[i];
                else
                    totalCost += weightChange * tradingCost_[i];
            }

            std::vector<AD<double> > Y(1);
            Y[0] = totalCost;
            fImpl_ = CppAD::ADFun<double>(X, Y);
        }

        void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad) {

            std::vector<double> x(variableNumber_);

            for (int i = 0; i != variableNumber_; ++i)
                x[i] = xWeight[i];

            std::vector<double> y = fImpl_.Forward(0, x);
            std::vector<double> jac = fImpl_.Jacobian(x);

            func = y[0];

            for (int i = 0; i != variableNumber_; ++i)
                grad[i] = jac[i];
        }

private:
    boost::numeric::ublas::vector<double> expectReturn_;
    boost::numeric::ublas::matrix<double> varMatrix_;
    boost::numeric::ublas::vector<double> tradingCost_;
    boost::numeric::ublas::vector<double> currentWeight_;
    int variableNumber_;
    CppAD::ADFun<double> fImpl_;
};


inline void calculate_cppad(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr) {
    ((CostCalculator_cppad*)ptr)->calculateCost(xWeight, func, grad);
}

//////////////////////////


class CostCalculator_adept {
public:
    CostCalculator_adept(const real_1d_array expectReturn,
        const real_2d_array& varMatrix,
        const real_1d_array& tradingCost,
        const real_1d_array& currentWeight) {
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

    void calculateCost(const real_1d_array& xWeight, double& func, real_1d_array& grad) {

        adept::Stack stack;
        using adept::adouble;

        boost::numeric::ublas::vector<adouble> realX(variableNumber_);

        for (int i = 0; i != variableNumber_; ++i)
            realX[i] = xWeight[i];

        stack.new_recording();

        adouble totalCost = 0.0;

        totalCost -= boost::numeric::ublas::inner_prod(expectReturn_, realX);

        totalCost += 0.5 * boost::numeric::ublas::inner_prod(
            boost::numeric::ublas::prod(varMatrix_, realX), realX);

        // ���� trading cost
        for (int i = 0; i != variableNumber_; ++i) {
            adouble weightChange = realX[i] - currentWeight_[i];
            if (weightChange < 0.)
                totalCost += -weightChange * tradingCost_[i];
            else
                totalCost += weightChange * tradingCost_[i];
        }

        totalCost.set_gradient(1.0);
        stack.compute_adjoint();

        func = totalCost.value();

        for (int i = 0; i != variableNumber_; ++i)
            grad[i] = realX[i].get_gradient();

    }

private:
    boost::numeric::ublas::vector<double> expectReturn_;
    boost::numeric::ublas::matrix<double> varMatrix_;
    boost::numeric::ublas::vector<double> tradingCost_;
    boost::numeric::ublas::vector<double> currentWeight_;
    int variableNumber_;
};

inline void calculate_adept(const real_1d_array& xWeight, double& func, real_1d_array& grad, void *ptr) {
    ((CostCalculator_adept*)ptr)->calculateCost(xWeight, func, grad);
}


int main(int argc, char **argv)
{
    real_2d_array varMatrix;
    alglib::read_csv("d:/20160303_1000.csv", ',', 0, varMatrix);

    int variableNumber = varMatrix.rows();

    // ���ý��׳ɱ�
    real_1d_array tradingCost;
    tradingCost.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        tradingCost[i] = 0.003;

    // ����Ԥ������
    real_1d_array expectReturn;
    expectReturn.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        expectReturn[i] = i / 5.0 /variableNumber;

    // ���õ�ǰ���Ȩ��
    real_1d_array currentWeight;
    currentWeight.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        currentWeight[i] = 1.0 / variableNumber;

    boost::timer timer;
    real_1d_array startWeight;
    startWeight.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        startWeight[i] = 1.0 / variableNumber;

    // constraints
    real_1d_array bndl;
    bndl.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        bndl[i] = 0.0;

    real_1d_array bndu;
    bndu.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        bndu[i] = 1.0;

    real_2d_array linearConMatrix;
    integer_1d_array condtion = "[0]";
    linearConMatrix.setlength(1, variableNumber + 1);
    for (int i = 0; i != variableNumber; ++i)
        linearConMatrix[0][i] = 1.0;
    linearConMatrix[0][variableNumber] = 1.0;

    // stop condition
    double epsg = 0.000001;
    double epsf = 0;
    double epsx = 0;
    alglib::ae_int_t maxits = 0;

    {
        CostCalculator_analytic costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        timer.restart();
        alglib::minbleicstate state_analytic;
        alglib::minbleicreport rep_analytic;

        alglib::minbleiccreate(startWeight, state_analytic);
        alglib::minbleicsetlc(state_analytic, linearConMatrix, condtion);
        alglib::minbleicsetbc(state_analytic, bndl, bndu);
        alglib::minbleicsetcond(state_analytic, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;
        alglib::minbleicoptimize(state_analytic, calculate_analytic, NULL, &costCalc);
        alglib::minbleicresults(state_analytic, targetWeight, rep_analytic);

        std::cout << "analytic method : " << timer.elapsed() << std::endl;
    }

    {
        CostCalculator_cppad costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        timer.restart();
        alglib::minbleicstate state_cppad;
        alglib::minbleicreport rep_cppad;

        alglib::minbleiccreate(startWeight, state_cppad);
        alglib::minbleicsetlc(state_cppad, linearConMatrix, condtion);
        alglib::minbleicsetbc(state_cppad, bndl, bndu);
        alglib::minbleicsetcond(state_cppad, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;
        alglib::minbleicoptimize(state_cppad, calculate_cppad, NULL, &costCalc);
        alglib::minbleicresults(state_cppad, targetWeight, rep_cppad);

        std::cout << "adjoint differentiation (cppad): " << timer.elapsed() << std::endl;
    }

    {
        CostCalculator_adept costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        timer.restart();
        alglib::minbleicstate state_ad;
        alglib::minbleicreport rep_ad;

        alglib::minbleiccreate(startWeight, state_ad);
        alglib::minbleicsetlc(state_ad, linearConMatrix, condtion);
        alglib::minbleicsetbc(state_ad, bndl, bndu);
        alglib::minbleicsetcond(state_ad, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;
        alglib::minbleicoptimize(state_ad, calculate_adept, NULL, &costCalc);
        alglib::minbleicresults(state_ad, targetWeight, rep_ad);

        std::cout << "adjoint differentiation (adept): " << timer.elapsed() << std::endl;
    }

    {
        CostCalculator_fd costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        timer.restart();

        double diffstep = 1.0e-6;
        alglib::minbleicstate state;
        alglib::minbleicreport rep;

        alglib::minbleiccreatef(startWeight, diffstep, state);
        alglib::minbleicsetlc(state, linearConMatrix, condtion);
        alglib::minbleicsetbc(state, bndl, bndu);
        alglib::minbleicsetcond(state, epsg, epsf, epsx, maxits);

        alglib::minbleicoptimize(state, calculate_fd, NULL, &costCalc);

        real_1d_array targetWeight;
        alglib::minbleicresults(state, targetWeight, rep);

        std::cout << "finite difference: " << timer.elapsed() << std::endl;
    }

    return 0;
}