/*
 * This is an example to show the performance of different methods with respect to the following ..
 *
 * Single period optimal portfolio allocation problem:
 *
 * C = \mathrm{argmin}_{w} ( \frac{1}{2} w^TCw + |w - \bar w|^TT - w^TR)
 *
 * subjected to:
 *
 * 0 < w_i < 1 for each i,
 * \sum_i w_i = 1
 *
 * where:
 *
 * w : the target portfolio allocation vector of different securities.
 * C : the covariance matrix of the whole securities universe.
 * T : trading cost vector.
 * R : the expected return vector.
 * \bar w : current portfolio allocation vector.
 *
 * for this problem we use single optimization method `minbleic` which are implemented in Alglib combined with
 * the following 4 ways to calculate cost function or its corresponding gradient:
 *
 * 1. `calculate_fd`
 *
 * Only cost function are provided to optimizer. The gradient are calculated by finite difference method which are handled by the optimizer
 * internally.
 *
 * 2. `calculate_analytic`
 *
 * Both cost function and its gradient are provided to optimizer. The gradient are calculated explicitly by hand-written codes.
 *
 * 3. `calculate_cppad`
 *
 * Both cost function and its gradient are provided to optimizer. The gradient are calculated using automatic differentiation tool CppAD,
 *
 * 4. `calculate_adept`
 *
 * Both cost function and its gradient are provided to optimizer. The gradient are calculated using automatic differentiation tool Adept,
 */

#include "ParameterReader.hpp"
#include "utilities.hpp"
#include "CostCalculator_fd.hpp"
#include "CostCalculator_analytic.hpp"
#include "CostCalculator_eigen.hpp"
#include "CostCalculator_adept.hpp"
#include "CostCalculator_cppad.hpp"
#include <boost/timer.hpp>
#include <optimization.h>
#include <iomanip>


int main(int argc, char **argv)
{

    /*
     * Read all the parameters from the file
     *
     * varMatrix: co-variance matrix of the securities.
     * tradingCost: trading costs vector of the securities.
     * expectReturn: expect returns vector of the securities.
     * currentWeight: current portfolio allocation vector of the securities.
     */

    int problemSize;
    std::cout << "Please input problem size (e.g. 100): ";
    std::cin >> problemSize;

    char buffer[100];

    // Please set the data file path here
    sprintf(buffer, "../../data/20160303_%d.csv", problemSize);
    std::string filaPath(buffer);

    boost::tuple<real_2d_array, real_1d_array, real_1d_array, real_1d_array>
        parameters;

    try 
    {
        parameters = parameterReader(filaPath);
    }
    catch (std::runtime_error& e)
    {
        std::cout << e.what() << std::endl;
        throw;
    }
    
    real_2d_array varMatrix = parameters.get<0>();
    real_1d_array tradingCost = parameters.get<1>();
    real_1d_array expectReturn = parameters.get<2>();
    real_1d_array currentWeight = parameters.get<3>();

    int variableNumber = varMatrix.rows();

    /*
     * Portfolio construction constraints
     *
     * 1. Bounded constraint: 0 < x_i < 1.0 for each i stands for one particular security.
     * 2. Linear constraint: \sum_i x_i = 1.0, no cash out and no leverage.
     */

    // bounded constraints
    real_1d_array bndl;
    bndl.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        bndl[i] = 0.0;

    real_1d_array bndu;
    bndu.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        bndu[i] = 1.0;

    // linear constraints
    real_2d_array conMatrix;
    integer_1d_array condType = "[0]";
    conMatrix.setlength(1, variableNumber + 1);
    for (int i = 0; i != variableNumber; ++i)
        conMatrix[0][i] = 1.0;
    conMatrix[0][variableNumber] = 1.0;

    // stop condition
    double epsg = 1e-8;
    double epsf = 1e-8;
    double epsx = 1e-8;
    alglib::ae_int_t maxits = 0;

    // start point
    real_1d_array startWeight;
    startWeight.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        startWeight[i] = 1.0 / variableNumber;

    //
    int widths[] = { 25, 14, 14, 14, 14, 14, 14 };
    std::cout << std::setw(widths[0]) << std::left << "Method"
        << std::setw(widths[1]) << std::left << "Time(s)"
        << std::setw(widths[2]) << std::left << "f(x)"
        << std::setw(widths[3]) << std::left << "FuncEval"
        << std::setw(widths[4]) << std::left << "min(x_i)"
        << std::setw(widths[5]) << std::left << "max(x_i)"
        << std::setw(widths[6]) << std::left << "sum(x_i)"
        << std::endl;


    boost::timer timer;

    {
        timer.restart();
        CostCalculator_analytic costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        alglib::minbleicstate state_analytic;
        alglib::minbleicreport rep_analytic;

        alglib::minbleiccreate(startWeight, state_analytic);
        alglib::minbleicsetlc(state_analytic, conMatrix, condType);
        alglib::minbleicsetbc(state_analytic, bndl, bndu);
        alglib::minbleicsetcond(state_analytic, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;

        alglib::minbleicoptimize(state_analytic, calculate_analytic, NULL, &costCalc);
        alglib::minbleicresults(state_analytic, targetWeight, rep_analytic);

        std::cout << std::setw(widths[0]) << std::left << "Alglib (analytic)"
                << std::fixed << std::setprecision(6)
                << std::setw(widths[1]) << std::left << timer.elapsed()
                << std::setw(widths[2]) << std::left << state_analytic.f
                << std::setw(widths[3]) << std::left << rep_analytic.nfev
                << std::setw(widths[4]) << std::left << min(targetWeight)
                << std::setw(widths[5]) << std::left << max(targetWeight)
                << std::setw(widths[6]) << std::left << sum(targetWeight)
                << std::endl;
    }

    {
        timer.restart();
        CostCalculator_eigen costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        alglib::minbleicstate state_eigen;
        alglib::minbleicreport rep_eigen;

        alglib::minbleiccreate(startWeight, state_eigen);
        alglib::minbleicsetlc(state_eigen, conMatrix, condType);
        alglib::minbleicsetbc(state_eigen, bndl, bndu);
        alglib::minbleicsetcond(state_eigen, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;

        alglib::minbleicoptimize(state_eigen, calculate_eigen, NULL, &costCalc);
        alglib::minbleicresults(state_eigen, targetWeight, rep_eigen);

        std::cout << std::setw(widths[0]) << std::left << "Eigen (analytic)"
        << std::fixed << std::setprecision(6)
        << std::setw(widths[1]) << std::left << timer.elapsed()
        << std::setw(widths[2]) << std::left << state_eigen.f
        << std::setw(widths[3]) << std::left << rep_eigen.nfev
        << std::setw(widths[4]) << std::left << min(targetWeight)
        << std::setw(widths[5]) << std::left << max(targetWeight)
        << std::setw(widths[6]) << std::left << sum(targetWeight)
        << std::endl;
    }

    {
        timer.restart();
        CostCalculator_adept costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        alglib::minbleicstate state_adept;
        alglib::minbleicreport rep_adept;

        alglib::minbleiccreate(startWeight, state_adept);
        alglib::minbleicsetlc(state_adept, conMatrix, condType);
        alglib::minbleicsetbc(state_adept, bndl, bndu);
        alglib::minbleicsetcond(state_adept, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;

        alglib::minbleicoptimize(state_adept, calculate_adept, NULL, &costCalc);
        alglib::minbleicresults(state_adept, targetWeight, rep_adept);

        std::cout << std::setw(widths[0]) << std::left << "AD (adept)"
                << std::fixed
                << std::setw(widths[1]) << std::left << timer.elapsed()
                << std::setw(widths[2]) << std::left << state_adept.f
                << std::setw(widths[3]) << std::left << rep_adept.nfev
                << std::setw(widths[4]) << std::left << min(targetWeight)
                << std::setw(widths[5]) << std::left << max(targetWeight)
                << std::setw(widths[6]) << std::left << sum(targetWeight)
                << std::endl;
    }

    {
        timer.restart();
        CostCalculator_cppad costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        alglib::minbleicstate state_cppad;
        alglib::minbleicreport rep_cppad;

        alglib::minbleiccreate(startWeight, state_cppad);
        alglib::minbleicsetlc(state_cppad, conMatrix, condType);
        alglib::minbleicsetbc(state_cppad, bndl, bndu);
        alglib::minbleicsetcond(state_cppad, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;

        alglib::minbleicoptimize(state_cppad, calculate_cppad, NULL, &costCalc);
        alglib::minbleicresults(state_cppad, targetWeight, rep_cppad);

        std::cout << std::setw(widths[0]) << std::left << "AD (cppad)"
                << std::fixed
                << std::setw(widths[1]) << std::left << timer.elapsed()
                << std::setw(widths[2]) << std::left << state_cppad.f
                << std::setw(widths[3]) << std::left << rep_cppad.nfev
                << std::setw(widths[4]) << std::left << min(targetWeight)
                << std::setw(widths[5]) << std::left << max(targetWeight)
                << std::setw(widths[6]) << std::left << sum(targetWeight)
                << std::endl;
    }

    {
        timer.restart();
        CostCalculator_fd costCalc(expectReturn, varMatrix, tradingCost, currentWeight);

        double diffstep = 1.0e-8;
        alglib::minbleicstate state_fd;
        alglib::minbleicreport rep_fd;

        alglib::minbleiccreatef(startWeight, diffstep, state_fd);
        alglib::minbleicsetlc(state_fd, conMatrix, condType);
        alglib::minbleicsetbc(state_fd, bndl, bndu);
        alglib::minbleicsetcond(state_fd, epsg, epsf, epsx, maxits);

        real_1d_array targetWeight;

        alglib::minbleicoptimize(state_fd, calculate_fd, NULL, &costCalc);
        alglib::minbleicresults(state_fd, targetWeight, rep_fd);

        std::cout << std::setw(widths[0]) << std::left << "Finite Difference"
                << std::fixed
                << std::setw(widths[1]) << std::left << timer.elapsed()
                << std::setw(widths[2]) << std::left << state_fd.f
                << std::setw(widths[3]) << std::left << rep_fd.nfev
                << std::setw(widths[4]) << std::left << min(targetWeight)
                << std::setw(widths[5]) << std::left << max(targetWeight)
                << std::setw(widths[6]) << std::left << sum(targetWeight)
                << std::endl;
    }

    return 0;
}