alglib 优化例子
========================

测试样例
------------------------

测试如下问题的求解：

带线性交易成本的单期资产优化问题：

C = \mathrm{argmin}_{w} ( \frac{1}{2} w^TCw + |w - \bar w|^TT - w^TR)

具有如下的限制条件：

* 0 < w_i < 1 for each i,
* \sum_i w_i = 1.

其中：

* w : 目标资产配置；
* C : 资产协方差矩阵；
* T : 单位交易成本；
* R : 预期收益；
* \bar w : 当前资产配置。


基于这个问题，例子中将测试 ``Alglib`` 在3种不同计算方案下的表现，同时给出了 ``Matlab`` 的对比用例。

运行例子
--------------------------

1. ``Alglib``

   * 进入根目录下 ``build`` 文件夹;

   * 使用Visual Studio打开 ``alglib.sln`` 文件;
    
   * 点选 ``生成`` -> ``生成解决方案``;
    
   * 运行 ``Optimization`` 项目，输入资产数目之后，会有类似如下的运行结果：

   
        Please input problem size (e.g. 100): 100
        Method                   Time(s)       f(x)          FuncEval      min(x_i)      max(x_i)      sum(x_i)
        Alglib (analytic)        0.002000      -0.131214     121           0.000000      0.651510      1.000000
        Eigen (analytic)         0.002000      -0.131214     121           0.000000      0.651510      1.000000

        ******************************************************************************
        This program contains Ipopt, a library for large-scale nonlinear optimization.
        Ipopt is released as open source code under the Eclipse Public License (EPL).
                For more information visit http://projects.coin-or.org/Ipopt
        ******************************************************************************

        NOTE: You are using Ipopt by default with the MUMPS linear solver.
        Other linear solvers might be more efficient (see Ipopt documentation).


        Ipopt (analytic)         0.063411      -0.019598     15            0.000000      0.093335      1.000000
        CUDA (analytic)          0.987000      -0.131214     121           0.000000      0.651510      1.000000
        AD (adept)               0.029000      -0.131214     121           0.000000      0.651510      1.000000
        AD (cppad)               0.052000      -0.131214     121           0.000000      0.651510      1.000000
        Finite Difference        0.214000      -0.131214     34281         0.000000      0.651510      1.000000


2. ``Matlab``

   * 进入根目录下``matlab``文件夹；
    
   * 运行 ``doOptimization.m`` 文件。
