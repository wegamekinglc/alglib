//
// Created by wegamkeinglc on 17-6-24.
//

#include "TestProblem.hpp"
#include "assert.h"

TestProblem::TestProblem(const real_1d_array &expectReturn, const real_2d_array &varMatrix, int n)
{
    assert(expectReturn.length() == varMatrix.rows());
    assert(varMatrix.rows() == varMatrix.cols());

    numOfAssets_ = n;

    xReal_.resize(numOfAssets_, 1);
    varMatrix_.resize(numOfAssets_, numOfAssets_);
    expectReturn_.resize(numOfAssets_);

    for (int i = 0; i != numOfAssets_; ++i) {
        expectReturn_(i) = expectReturn[i];
        for (int j = 0; j != numOfAssets_; ++j)
            varMatrix_(i, j) = varMatrix[i][j];
    }

    fcount_ = 0;
    grad_f_.setlength(numOfAssets_);
    x_.setlength(numOfAssets_);

}

bool TestProblem::setBoundedConstraint(const real_1d_array& lb, const real_1d_array& ub)
{
    assert(lb.length() == numOfAssets_);
    assert(ub.length() == numOfAssets_);

    lb_ = lb;
    ub_ = ub;

    return true;
}

bool TestProblem::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
                               Index& nnz_h_lag, IndexStyleEnum& index_style)
{
    n = numOfAssets_;
    m = 1;
    nnz_jac_g = numOfAssets_;
    index_style = TNLP::C_STYLE;
    return true;
}

bool TestProblem::get_bounds_info(Index n, Number* x_l, Number* x_u,
                                  Index m, Number* g_l, Number* g_u)
{

    for (Index i = 0; i < numOfAssets_; ++i)
    {
        x_l[i] = lb_[i];
        x_u[i] = ub_[i];
    }

    g_l[0] = g_u[0] = 1.;
    return true;
}

bool TestProblem::get_starting_point(Index n, bool init_x, Number* x,
                                     bool init_z, Number* z_L, Number* z_U,
                                     Index m, bool init_lambda,
                                     Number* lambda)
{
    double startWeight = 1. / numOfAssets_;
    for (Index i = 0; i < numOfAssets_; ++i)
    {
        x[i] = startWeight;
    }
    return true;
}

bool TestProblem::eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
{
    fcount_ += 1;
    for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];

    // risk grad
    VectorXd riskGrad = varMatrix_ * xReal_;

    obj_value = 0.5 * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
    VectorXd tmp = riskGrad - expectReturn_;


    for (int i = 0; i < numOfAssets_; ++i)
    {
        grad_f_(i) = tmp(i);
    }
    return true;
}

bool TestProblem::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
{
    if (new_x) {
        for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];

        // risk grad
        VectorXd riskGrad = varMatrix_ * xReal_;

        VectorXd tmp = riskGrad - expectReturn_;

        for (int i = 0; i < numOfAssets_; ++i)
        {
            grad_f[i] = grad_f_(i) = tmp(i);
        }
    }
    else
        for (Index i = 0; i < n; ++i) grad_f[i] = grad_f_(i);

    return true;
}

bool TestProblem::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
{

    g[0] = 0.;
    for (Index i = 0; i < n; ++i)
        g[0] += x[i];
    return true;
}

bool TestProblem::eval_jac_g(Index n, const Number* x, bool new_x,
                             Index m, Index nele_jac, Index* iRow, Index *jCol,
                             Number* values)
{
    if (values == NULL) {
        for (Index i = 0; i < numOfAssets_; ++i)
        {
            iRow[i] = 0;
            jCol[i] = i;
        }
    }
    else {
        for (Index i = 0; i < numOfAssets_; ++i)
        {
            for (Index i = 0; i < numOfAssets_; ++i)
            {
                values[i] = 1.;
            }
        }
    }

    return true;
}

void TestProblem::finalize_solution(SolverReturn status,
                                    Index n, const Number* x, const Number* z_L, const Number* z_U,
                                    Index m, const Number* g, const Number* lambda,
                                    Number obj_value,
                                    const IpoptData* ip_data,
                                    IpoptCalculatedQuantities* ip_cq)
{
    for (Index i = 0; i < n; ++i)
        x_[i] = x[i];
    feval_ = obj_value;
}