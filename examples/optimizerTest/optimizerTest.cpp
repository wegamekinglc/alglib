#include "ParameterReader.hpp"
#include <iostream>

extern "C" void portfolioOptimizer(int size, double* covMatrix, double* expectedReturn, double* tradingCost, double* currentWeight, double* targetWeight, double* cost);

int main()
{
    int problemSize;
    std::cout << "Please input problem size (e.g. 100): ";
    std::cin >> problemSize;

    char buffer[100];

    // Please set the data file path here
    sprintf(buffer, "../../../data/20160303_%d.csv", problemSize);
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

    int size = varMatrix.rows();
    double* cov = new double[size*size];
    alglib::vmove(&cov[0], &varMatrix[0][0], size * size);
    for(int i=0;i != size; ++i){
        for(int j=0; j!= size; ++j)
            cov[i*size + j] = varMatrix[i][j];
    }

    double* expect = new double[size];
    alglib::vmove(&expect[0], &expectReturn[0], size);
    double* t = new double[size];
    alglib::vmove(&t[0], &tradingCost[0], size);
    double* w = new double[size];
    alglib::vmove(&w[0], &currentWeight[0], size);

    double* res= new double[size];
    double* cost = new double[1];

    portfolioOptimizer(size, cov, expect, t, w, res, cost);

    std::cout << cost[0] << std::endl;

    delete cov;
    delete expect;
    delete t;
    delete w;
    delete res;
    delete cost;

    return 0;
}

