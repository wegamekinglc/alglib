#include "ipopt_optimizer.hpp"
#include "assert.h"


PP_Problem::PP_Problem(const real_1d_array& expectReturn,
	const real_2d_array &varMatrix,
	const real_1d_array &tradingCost,
	const real_1d_array &currentWeight,
	const double& tcBuget)
	: tcBuget_(tcBuget)
{
	assert(expectReturn.length() == varMatrix.rows());
	assert(varMatrix.rows() == varMatrix.cols());
	assert(tradingCost.length() == varMatrix.rows());
	assert(currentWeight.length() == varMatrix.rows());
	numOfAssets_ = expectReturn.length();

	xReal_.resize(numOfAssets_, 1);
	varMatrix_.resize(numOfAssets_, numOfAssets_);
	expectReturn_.resize(numOfAssets_);
	tradingCost_.resize(numOfAssets_);
	currentWeight_.resize(numOfAssets_);

	for (int i = 0; i != numOfAssets_; ++i) {
		expectReturn_(i) = expectReturn[i];
		tradingCost_(i) = tradingCost[i];
		currentWeight_(i) = currentWeight[i];
		for (int j = 0; j != numOfAssets_; ++j)
			varMatrix_(i, j) = varMatrix[i][j];
	}

	fcount_ = 0;
	grad_f_.setlength(2 * numOfAssets_);
	x_.setlength(2 * numOfAssets_);
}

bool PP_Problem::setBoundedConstraint(const real_1d_array& lb, const real_1d_array& ub)
{
	assert(lb.length() == numOfAssets_);
	assert(ub.length() == numOfAssets_);

	lb_ = lb;
	ub_ = ub;

	return true;
}

bool PP_Problem::setLinearConstraint(const real_2d_array& conMatrix, const integer_1d_array& conType)
{
	assert(conMatrix.rows() == conType.length());
	assert(conMatrix.cols() == numOfAssets_ + 1);

	conMatrix_ = conMatrix;
	conType_ = conType;

	return true;
}

bool PP_Problem::get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
	Index& nnz_h_lag, IndexStyleEnum& index_style)
{
	n = 2 * numOfAssets_;
	m = conMatrix_.rows() + 1 + 2 * numOfAssets_;
	nnz_jac_g = conMatrix_.rows() * numOfAssets_ + 5 * numOfAssets_;
	index_style = TNLP::C_STYLE;
	return true;
}

bool PP_Problem::get_bounds_info(Index n, Number* x_l, Number* x_u,
	Index m, Number* g_l, Number* g_u)
{
	
	int ext_linear = conMatrix_.rows();

	if (lb_.length() == numOfAssets_)
	{
		for (Index i = 0; i < numOfAssets_; ++i)
		{
			x_l[i] = lb_[i];
			x_u[i] = ub_[i];
			x_l[i + numOfAssets_] = -1e19;
			x_u[i + numOfAssets_] = 1e19;
		}
	}

	for (int i = 0; i < ext_linear; ++i)
	{
		if (conType_[i] == 0)
			g_l[i] = g_u[i] = conMatrix_[i][numOfAssets_];
		else if (conType_[i] < 0)
		{
			g_l[i] = -1e19;
			g_u[i] = conMatrix_[i][numOfAssets_];
		}
		else
		{
			g_l[i] = conMatrix_[i][numOfAssets_];
			g_u[i] = 1e19;
		}
	}

	g_l[ext_linear] = -1e19;
	g_u[ext_linear] = tcBuget_;

	for (Index i = 0; i < numOfAssets_; ++i)
	{
		g_l[1 + ext_linear + 2 * i] = -1e19;
		g_u[1 + ext_linear + 2 * i] = currentWeight_[i];

		g_l[2 + ext_linear + 2 * i] = currentWeight_[i];
		g_u[2 + ext_linear + 2 * i] = 1e19;
	}

	return true;
}

bool PP_Problem::get_starting_point(Index n, bool init_x, Number* x,
	bool init_z, Number* z_L, Number* z_U,
	Index m, bool init_lambda,
	Number* lambda)
{
	for (Index i = 0; i < numOfAssets_; ++i)
	{
		x[i] = currentWeight_[i];
		x[i + numOfAssets_] = 0.0;
	}
	return true;
}

bool PP_Problem::eval_f(Index n, const Number* x, bool new_x, Number& obj_value)
{
	fcount_ += 1;
	for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];

	// risk grad
	VectorXd riskGrad = varMatrix_ * xReal_;

	obj_value = 0.5 * xReal_.dot(riskGrad) - expectReturn_.dot(xReal_);
	VectorXd tmp = riskGrad - expectReturn_;


	for (int i = 0; i < numOfAssets_; ++i)
	{
		// trading cost
		obj_value += x[i + numOfAssets_];

		grad_f_(i) = tmp(i);
		grad_f_(i + numOfAssets_) = 1.0;
	}
	return true;
}


bool PP_Problem::eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f)
{
	if (new_x) {
		for (int i = 0; i < numOfAssets_; ++i) xReal_(i) = x[i];

		// risk grad
		VectorXd riskGrad = varMatrix_ * xReal_;

		VectorXd tmp = riskGrad - expectReturn_;

		for (int i = 0; i < numOfAssets_; ++i)
		{
			grad_f[i] = grad_f_(i) = tmp(i);
			grad_f[i + numOfAssets_] = grad_f_(i + numOfAssets_) = 1.0;
		}
	}
	else
		for (Index i = 0; i < n; ++i) grad_f[i] = grad_f_(i);

	return true;
}

bool PP_Problem::eval_g(Index n, const Number* x, bool new_x, Index m, Number* g)
{
	int ext_linear = conMatrix_.rows();
	
	for (Index i = 0; i < m; ++i)
		g[i] = 0.0;

	for (Index i = 0; i < numOfAssets_; ++i) {

		for (int j = 0; j < ext_linear; ++j)
			g[j] += conMatrix_[j][i] * x[i];

		g[ext_linear] += x[i + numOfAssets_];

		g[ext_linear + 1 + 2 * i] = x[i] - x[i + numOfAssets_] / tradingCost_[i];
		g[ext_linear + 2 + 2 * i] = x[i] + x[i + numOfAssets_] / tradingCost_[i];
	}
	return true;
}

bool PP_Problem::eval_jac_g(Index n, const Number* x, bool new_x,
	Index m, Index nele_jac, Index* iRow, Index *jCol,
	Number* values)
{
	int ext_linear = conMatrix_.rows();
	if (values == NULL) {
		for (Index i = 0; i < numOfAssets_; ++i)
		{
			for (Index j = 0; j < ext_linear; ++j)
			{
				iRow[i + j * numOfAssets_] = j;
				jCol[i + j * numOfAssets_] = i;
			}

			iRow[i + ext_linear * numOfAssets_] = ext_linear;
			jCol[i + ext_linear * numOfAssets_] = i + numOfAssets_;

			iRow[4 * i + (ext_linear + 1) * numOfAssets_] = 2 * i + ext_linear + 1;
			jCol[4 * i + (ext_linear + 1) * numOfAssets_] = i;

			iRow[4 * i + 1 + (ext_linear + 1) * numOfAssets_] = 2 * i + ext_linear + 1;
			jCol[4 * i + 1 + (ext_linear + 1) * numOfAssets_] = i + numOfAssets_;

			iRow[4 * i + 2 + (ext_linear + 1) * numOfAssets_] = 2 * i + ext_linear + 2;
			jCol[4 * i + 2 + (ext_linear + 1) * numOfAssets_] = i;

			iRow[4 * i + 3 + (ext_linear + 1) * numOfAssets_] = 2 * i + ext_linear + 2;
			jCol[4 * i + 3 + (ext_linear + 1) * numOfAssets_] = i + numOfAssets_;
		}
	}
	else {
		for (Index i = 0; i < numOfAssets_; ++i)
		{
			for (Index j = 0; j < ext_linear; ++j)
			{
				values[i + j * numOfAssets_] = conMatrix_[j][i];
			}
			values[i + ext_linear * numOfAssets_] = 1.0;
			values[4 * i + (ext_linear + 1) * numOfAssets_] = 1.0;
			values[4 * i + 1 + (ext_linear + 1) * numOfAssets_] = -1.0 / tradingCost_[i];
			values[4 * i + 2 + (ext_linear + 1) * numOfAssets_] = 1.0;
			values[4 * i + 3 + (ext_linear + 1) * numOfAssets_] = 1.0 / tradingCost_[i];
		}
	}

	return true;
}


void PP_Problem::finalize_solution(SolverReturn status,
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