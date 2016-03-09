#include "optimizer.hpp"

#include <ap.h>
#include "costcalculator.hpp"
#include <optimization.h>

using namespace alglib;

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
                        double* cost)
{
    real_2d_array cov;
    cov.setcontent(size, size, &covMatrix[0]);

    real_1d_array r;
    r.setcontent(size, &expectedReturn[0]);

    real_1d_array t;
    t.setcontent(size, &tradingCost[0]);

    real_1d_array w;
    w.setcontent(size, &currentWeight[0]);

    double epsg = 1e-8;
    double epsf = 1e-8;
    double epsx = 1e-8;
    alglib::ae_int_t maxits = 0;

    CostCalculator calc(r, cov, t, w);

    alglib::minbleicstate state_analytic;
    alglib::minbleicreport rep_analytic;

    alglib::minbleiccreate(w, state_analytic);

    alglib::minbleicsetcond(state_analytic, epsg, epsf, epsx, maxits);

    // bounded constraints
    real_1d_array bndl;
    real_1d_array bndu;

    if(lowerBound != NULL && upperBound != NULL)
    {
        bndl.setcontent(size, &lowerBound[0]);
        bndu.setcontent(size, &upperBound[0]);
        alglib::minbleicsetbc(state_analytic, bndl, bndu);
    }

    // linear constraints
    real_2d_array conMatrix;
    integer_1d_array condType;
    if(linearCond != NULL && linearCondType != NULL)
    {
        conMatrix.setcontent(lcNumber, size + 1, &linearCond[0]);
        condType.setlength(lcNumber);
        for(int i=0; i!=lcNumber; ++i)
            condType[i] = linearCondType[i];
        alglib::minbleicsetlc(state_analytic, conMatrix, condType);
    }

    real_1d_array res;
    alglib::minbleicoptimize(state_analytic, calculate, NULL, &calc);
    alglib::minbleicresults(state_analytic, res, rep_analytic);

    alglib::vmove(&targetWeight[0], &res[0], size);
    cost[0] = state_analytic.f;
}

