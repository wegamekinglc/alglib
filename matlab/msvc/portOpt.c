#include "mex.h"

void portfolioOptimizer(int size,
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

void mexFunction(int nlhs, mxArray *plhs[], int nrhs, const mxArray *prhs[])
{
    int m = mxGetNumberOfElements(prhs[1]);
    double* var = mxGetPr(prhs[0]);
    double* exp = mxGetPr(prhs[1]);
    double* tct = mxGetPr(prhs[2]);
    double* cwt = mxGetPr(prhs[3]);
    
    int lc = 1;
    double* lb = NULL;
    double* ub = NULL;
    double* linearCond = NULL;
    int* linearCondType = NULL;
    
    plhs[0] = mxCreateDoubleMatrix(1, 1, mxREAL);
    plhs[1] = mxCreateDoubleMatrix(1, m, mxREAL);
    double* ct = mxGetPr(plhs[0]);
    double* tw = mxGetPr(plhs[1]);
    portfolioOptimizer(m, var, exp, tct, cwt, lb, ub, lc, linearCond, linearCondType, tw, ct); 
}