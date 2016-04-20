#include "optimizer.hpp"

#include <ap.h>
#include "costcalculator.hpp"
#include "ipopt_optimizer.hpp"
#include <optimization.h>
#include "IpIpoptApplication.hpp"

using namespace alglib;

static double global_epsg = 1e-8;
static double global_epsf = 1e-8;
static double global_epsx = 1e-8;
static alglib::ae_int_t global_maxits = 0;


void setStopCondition(double epsg, double epsf, double epsx, int maxits)
{
    global_epsg = epsg;
    global_epsf = epsf;
    global_epsx = epsx;
    global_maxits = maxits;
}


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

    double epsg = global_epsg;
    double epsf = global_epsf;
    double epsx = global_epsx;
    alglib::ae_int_t maxits = global_maxits;

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


void portfolioOptimizerWithTradingCostBudget(int size,
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

	double epsg = global_epsg;
	alglib::ae_int_t maxits = global_maxits;

	Ipopt::SmartPtr<PP_Problem> mynlp = new PP_Problem(r, cov, t, w, tradingCostBuget);
	
	// bounded constraints
	real_1d_array bndl;
	real_1d_array bndu;

	if (lowerBound != NULL && upperBound != NULL)
	{
		bndl.setcontent(size, &lowerBound[0]);
		bndu.setcontent(size, &upperBound[0]);
		mynlp->setBoundedConstraint(bndl, bndu);
	}

	// linear constraints
	real_2d_array conMatrix;
	integer_1d_array condType;
	if (linearCond != NULL && linearCondType != NULL)
	{
		conMatrix.setcontent(lcNumber, size + 1, &linearCond[0]);
		condType.setlength(lcNumber);
		for (int i = 0; i != lcNumber; ++i)
			condType[i] = linearCondType[i];
		mynlp->setLinearConstraint(conMatrix, condType);
	}

	Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();

	app->Options()->SetNumericValue("tol", epsg);
	app->Options()->SetIntegerValue("print_level", 0);
	app->Options()->SetStringValue("hessian_approximation", "limited-memory");
	app->Options()->SetIntegerValue("limited_memory_max_history", 3);

	Ipopt::ApplicationReturnStatus status = app->Initialize();
	status = app->OptimizeTNLP(mynlp);

	real_1d_array res = mynlp->xValue();
	alglib::vmove(&targetWeight[0], &res[0], size);
	cost[0] = mynlp->feval();
}