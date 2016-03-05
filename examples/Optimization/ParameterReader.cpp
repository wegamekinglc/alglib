#include "ParameterReader.hpp"

boost::tuple<real_2d_array, real_1d_array, real_1d_array, real_1d_array>
        parameterReader(const std::string& filePath)
{
    real_2d_array varMatrix;
    alglib::read_csv(filePath.c_str(), ',', 0, varMatrix);

    int variableNumber = varMatrix.rows();

    // set trading cost assumption
    real_1d_array tradingCost;
    tradingCost.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        tradingCost[i] = 0.003;

    // set expect return assumption
    real_1d_array expectReturn;
    expectReturn.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        expectReturn[i] = i / 5.0 /variableNumber;

    // set current weight assumption
    real_1d_array currentWeight;
    currentWeight.setlength(variableNumber);
    for (int i = 0; i != variableNumber; ++i)
        currentWeight[i] = 1.0 / variableNumber;

    return boost::make_tuple(varMatrix, tradingCost, expectReturn, currentWeight);
};

