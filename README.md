# alglib 优化例子

## 测试样例

测试如下问题的求解：

带线性交易成本的单期资产优化问题：

$$
\begin{aligned}
    C = \mathrm{argmin}_{w} ( \frac{1}{2} w^TCw + |w - \bar w|^TT - w^TR)
\end{aligned}
$$

具有如下的限制条件：

* $0 < w_i < 1 for each i,$
* $\sum_i w_i = 1.$

其中：

* w : 目标资产配置；
* C : 资产协方差矩阵；
* T : 单位交易成本；
* R : 预期收益；
* $\bar w$ : 当前资产配置。


基于这个问题，例子中将测试 ``Alglib`` 在多种不同计算方案下的表现，同时给出了 ``Matlab`` 的对比用例。

## 安装

1. Window

    将``ipopt/lib``目录加入环境变量``PATH``。

2. Linux

    确保已经安装``ipopt``。

## 运行例子

1. ``Alglib``

   * 进入根目录下 ``build`` 文件夹;

   * 使用Visual Studio打开 ``alglib.sln`` 文件;

   * 点选 ``生成`` -> ``生成解决方案``;

   * 运行 ``Optimization`` 项目，输入资产数目之后，会有类似如下的运行结果：

   ```
        Please input problem size (e.g. 100): 500
        Method                   Time(s)       f(x)          FuncEval      min(x_i)      max(x_i)      sum(x_i)
        Alglib (analytic)        0.082000      -0.146877     580           0.000000      0.232328      1.000000
        Eigen (analytic)         0.060000      -0.146877     580           0.000000      0.232328      1.000000

        ******************************************************************************
        This program contains Ipopt, a library for large-scale nonlinear optimization.
        Ipopt is released as open source code under the Eclipse Public License (EPL).
                For more information visit http://projects.coin-or.org/Ipopt
        ******************************************************************************

        NOTE: You are using Ipopt by default with the MUMPS linear solver.
        Other linear solvers might be more efficient (see Ipopt documentation).


        Ipopt (analytic)         0.593593      -0.146880     30            0.000000      0.233892      1.000005
        CUDA (analytic)          1.027000      -0.146877     580           0.000000      0.232328      1.000000
        AD (adept)               2.616000      -0.146877     580           0.000000      0.232328      1.000000
        AD (cppad)               5.430000      -0.146877     580           0.000000      0.232328      1.000000
        Finite Difference        81.528000     -0.146878     842645        0.000000      0.232655      1.000000
    ```


2. ``Matlab``
   
   * 将``ipopt/lib``加入``MATLAB``的搜索目录；

   * 进入根目录下``matlab/msvc``文件夹；

   * 运行 ``example_101.m`` 文件。
