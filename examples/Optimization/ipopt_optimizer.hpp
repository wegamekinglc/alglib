//
// Created by root on 16-4-2.
//

#ifndef ALGLIB_OPTIMIZATION_IPOPT_OPTIMIZER_HPP
#define ALGLIB_OPTIMIZATION_IPOPT_OPTIMIZER_HPP

#include <IpTNLP.hpp>

using Ipopt::TNLP;
using Ipopt::Number;
using Ipopt::Index;
using Ipopt::SolverReturn;
using Ipopt::IpoptData;
using Ipopt::IpoptCalculatedQuantities;

#ifdef USE_MKL_LIB
#define EIGEN_USE_MKL_ALL
#endif

#include "types.hpp"
#include <Eigen/Eigen>

using Eigen::MatrixXd;
using Eigen::VectorXd;


class PP_Problem : public TNLP
{
public:

	PP_Problem(const real_1d_array& expectReturn,
		const real_2d_array& varMatrix,
		const real_1d_array& tradingCost,
		const real_1d_array& currentWeight,
		const double& tcBuget);

	double tradingCost() const
	{
		double res = 0.0;
		for (Index i = 0; i < numOfAssets_; ++i) {
			const double change = x_[i] - currentWeight_[i];
			if (change > 0)
				res += change * tradingCost_[i];
			else
				res -= change * tradingCost_[i];
		}
		return res;
	}

	bool setBoundedConstraint(const real_1d_array& lb, const real_1d_array& ub);

	bool setLinearConstraint(const real_2d_array& conMatrix, const integer_1d_array& conType);

	virtual bool get_nlp_info(Index& n, Index& m, Index& nnz_jac_g,
		Index& nnz_h_lag, IndexStyleEnum& index_style);

	virtual bool get_bounds_info(Index n, Number* x_l, Number* x_u,
		Index m, Number* g_l, Number* g_u);

	virtual bool get_starting_point(Index n, bool init_x, Number* x,
		bool init_z, Number* z_L, Number* z_U,
		Index m, bool init_lambda,
		Number* lambda);

	virtual bool eval_f(Index n, const Number* x, bool new_x, Number& obj_value);

	virtual bool eval_grad_f(Index n, const Number* x, bool new_x, Number* grad_f);

	virtual bool eval_g(Index n, const Number* x, bool new_x, Index m, Number* g);

	virtual bool eval_jac_g(Index n, const Number* x, bool new_x,
		Index m, Index nele_jac, Index* iRow, Index *jCol,
		Number* values);

	virtual void finalize_solution(SolverReturn status,
		Index n, const Number* x, const Number* z_L, const Number* z_U,
		Index m, const Number* g, const Number* lambda,
		Number obj_value,
		const IpoptData* ip_data,
		IpoptCalculatedQuantities* ip_cq);

	Number feval() const { return feval_; }
	real_1d_array xValue() const { return x_; }
	Index fcount() const { return fcount_; }

private:
	VectorXd expectReturn_;
	MatrixXd varMatrix_;
	VectorXd tradingCost_;
	VectorXd currentWeight_;
	int numOfAssets_;
	VectorXd xReal_;

	real_1d_array grad_f_;
	real_1d_array x_;
	real_1d_array lb_;
	real_1d_array ub_;
	real_2d_array conMatrix_;
	integer_1d_array conType_;
	Number feval_;
	Index fcount_;
	double tcBuget_;
};


#endif //ALGLIB_IPOPT_OPTIMIZER_HPP
