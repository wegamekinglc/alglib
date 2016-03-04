#include "stdafx.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "optimization.h"

using namespace alglib;
void function1_grad(const real_1d_array &x, double &func, real_1d_array &grad, void *ptr)
{
    //
    // this callback calculates f(x0,x1) = 100*(x0+3)^4 + (x1-3)^4
    // and its derivatives df/d0 and df/dx1
    //
    func = 100 * pow(x[0] + 3, 4) + pow(x[1] - 3, 4);
    grad[0] = 400 * pow(x[0] + 3, 3);
    grad[1] = 4 * pow(x[1] - 3, 3);
}

int main(int argc, char **argv)
{
    //
    // This example demonstrates minimization of f(x,y) = 100*(x+3)^4+(y-3)^4
    // subject to inequality constraints:
    // * x>=2 (posed as general linear constraint),
    // * x+y>=6
    // using BLEIC optimizer.
    //
    real_1d_array x = "[5,5]";
    real_2d_array c = "[[1,0,2],[1,1,6]]";
    integer_1d_array ct = "[1,1]";
    minbleicstate state;
    minbleicreport rep;

    //
    // These variables define stopping conditions for the optimizer.
    //
    // We use very simple condition - |g|<=epsg
    //
    double epsg = 0.000001;
    double epsf = 0;
    double epsx = 0;
    ae_int_t maxits = 0;

    //
    // Now we are ready to actually optimize something:
    // * first we create optimizer
    // * we add linear constraints
    // * we tune stopping conditions
    // * and, finally, optimize and obtain results...
    //
    minbleiccreate(x, state);
    minbleicsetlc(state, c, ct);
    minbleicsetcond(state, epsg, epsf, epsx, maxits);
    alglib::minbleicoptimize(state, function1_grad);
    minbleicresults(state, x, rep);

    //
    // ...and evaluate these results
    //
    printf("%d\n", int(rep.terminationtype)); // EXPECTED: 4
    printf("%s\n", x.tostring(2).c_str()); // EXPECTED: [2,4]
    return 0;
}