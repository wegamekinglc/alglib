#include "optimizer.hpp"

#include <ap.h>
#include "costcalculator.hpp"
#include <optimization.h>

using namespace alglib;

void portfolioOptimizer(int size, double* covMatrix, double* expectedReturn, double* tradingCost, double* currentWeight, double* targetWeight, double* cost)
{
    real_2d_array cov;
    cov.setcontent(size, size, &covMatrix[0]);

    real_1d_array r;
    r.setcontent(size, &expectedReturn[0]);

    real_1d_array t;
    t.setcontent(size, &tradingCost[0]);

    real_1d_array w;
    w.setcontent(size, &currentWeight[0]);

    // bounded constraints
    real_1d_array bndl;
    bndl.setlength(size);
    for (int i = 0; i != size; ++i)
        bndl[i] = 0.0;

    real_1d_array bndu;
    bndu.setlength(size);
    for (int i = 0; i != size; ++i)
        bndu[i] = 1.0;

    // linear constraints
    real_2d_array conMatrix;
    integer_1d_array condType = "[0]";
    conMatrix.setlength(1, size + 1);
    for (int i = 0; i != size; ++i)
        conMatrix[0][i] = 1.0;
    conMatrix[0][size] = 1.0;

    double epsg = 1e-8;
    double epsf = 1e-8;
    double epsx = 1e-8;
    alglib::ae_int_t maxits = 0;

    CostCalculator calc(r, cov, t, w);

    alglib::minbleicstate state_analytic;
    alglib::minbleicreport rep_analytic;

    alglib::minbleiccreate(w, state_analytic);
    alglib::minbleicsetlc(state_analytic, conMatrix, condType);
    alglib::minbleicsetbc(state_analytic, bndl, bndu);
    alglib::minbleicsetcond(state_analytic, epsg, epsf, epsx, maxits);

    real_1d_array res;

    alglib::minbleicoptimize(state_analytic, calculate, NULL, &calc);
    alglib::minbleicresults(state_analytic, res, rep_analytic);

    alglib::vmove(&targetWeight[0], &res[0], size);

    real_1d_array grad;
    grad.setlength(size);

    cost[0] = state_analytic.f;
}

