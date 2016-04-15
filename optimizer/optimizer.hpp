#ifndef ALGLIB_OPTIMIZER_OPTIMIZER_HPP
#define ALGLIB_OPTIMIZER_OPTIMIZER_HPP

#ifdef _MSC_VER
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT
#endif


extern "C" void DLL_EXPORT setStopCondition(double epsg, double epsf, double epsx, int maxits);

extern "C" void DLL_EXPORT portfolioOptimizer(int size,
                                              double* covMatrix,
                                              double* expectedReturn,
                                              double* tradingCost,
                                              double* currentWeight,
                                              double* lowerBound,
                                              double* upperBound,
                                              int lcNumber,
                                              double* linearCond,
                                              int* linearCondType,
                                              double* targetWeight,
                                              double* cost);

extern "C" void DLL_EXPORT portfolioOptimizerWithTradingCostBudget(int size,
											  double* covMatrix,
											  double* expectedReturn,
											  double* tradingCost,
											  double* currentWeight,
											  double tradingCostBuget,
											  double* lowerBound,
											  double* upperBound,
											  int lcNumber,
											  double* linearCond,
											  int* linearCondType,
											  double* targetWeight,
											  double* cost);

#endif //ALGLIB_OPTIMIZER_OPTIMIZER_HPP
