Introduction
==========================

--------------------------
1.``portfolioOptimizer``
--------------------------

$$
\begin{aligned}
	C = \mathrm{argmin}_{w} ( \frac{1}{2} w^TCw + |w - \bar w|^TT - w^TR)
\end{aligned}
$$

subjected to:
 
$$
\begin{aligned}
	l_i & < w_i < u_i, \\\\
	Aw & \le | = | \ge b, \\\\
\end{aligned}
$$

where:

$$
\begin{aligned}
	w : & \text{the target portfolio allocation vector of different securities.} \\\\
	C : & \text{the covariance matrix of the whole securities universe.} \\\\
	T : & \text{trading cost vector.} \\\\
	R : & \text{the expected return vector.} \\\\
	\bar w : & \text{current portfolio allocation vector.} \\\\
	A : & p \star n \ \text{matrix}
\end{aligned}
$$

-------------------------------------------
2. `portfolioOptimizerWithTradingCostBudget`
--------------------------------------------

$$
\begin{aligned}
	C = \mathrm{argmin}_{w} ( \frac{1}{2} w^TCw + |w - \bar w|^TT - w^TR)
\end{aligned}
$$

subjected to:
 
$$
\begin{aligned}
	l_i & < w_i < u_i, \\\\
	Aw & \le | = | \ge b, \\\\
	|w - \bar w|^TT  & \le \eta
\end{aligned}
$$

where:

$$
\begin{aligned}
	w : & \text{the target portfolio allocation vector of different securities.} \\\\
	C : & \text{the covariance matrix of the whole securities universe.} \\\\
	T : & \text{trading cost vector.} \\\\
	R : & \text{the expected return vector.} \\\\
	\bar w : & \text{current portfolio allocation vector.} \\\\
	A : & p \star n \ \text{matrix} \\\\
	\eta : & \text{total trading cost budget}
\end{aligned}
$$