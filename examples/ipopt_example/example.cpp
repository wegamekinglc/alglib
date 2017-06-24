#include "TestProblem.hpp"
#include <boost/chrono.hpp>
#include "IpIpoptApplication.hpp"
#include "utilities.hpp"
#include <iomanip>

int main() {


    real_2d_array varMatrix;
    alglib::read_csv("../data/sec_cov_values.csv", ',', 0, varMatrix);

    real_2d_array tmp;
    alglib::read_csv("../data/signal.csv", ',', 0, tmp);

    real_1d_array expectReturn;
    int variableNumber = varMatrix.rows();
    expectReturn.setlength(variableNumber);
    for(int i=0; i != variableNumber; ++i)
        expectReturn[i] = tmp[i][0];

    for(int n=200; n <= 3000; n += 200) {
        real_1d_array bndl;
        bndl.setlength(n);
        for (int i = 0; i != n; ++i)
            bndl[i] = 0.0;

        real_1d_array bndu;
        bndu.setlength(n);
        for (int i = 0; i != n; ++i)
            bndu[i] = 1. / n * 30;

//        int widths[] = {25, 14, 14, 14, 14, 14, 14};
//        std::cout << std::setw(widths[0]) << std::left << "Method"
//                  << std::setw(widths[1]) << std::left << "Time(s)"
//                  << std::setw(widths[2]) << std::left << "f(x)"
//                  << std::setw(widths[3]) << std::left << "FuncEval"
//                  << std::setw(widths[4]) << std::left << "min(x_i)"
//                  << std::setw(widths[5]) << std::left << "max(x_i)"
//                  << std::setw(widths[6]) << std::left << "sum(x_i)"
//                  << std::endl;

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                start = boost::chrono::high_resolution_clock::now();
        Ipopt::SmartPtr<TestProblem> mynlp = new TestProblem(expectReturn, varMatrix, n);

        mynlp->setBoundedConstraint(bndl, bndu);

        Ipopt::SmartPtr<Ipopt::IpoptApplication> app = IpoptApplicationFactory();
        app->Options()->SetIntegerValue("print_level", 0);
        app->Options()->SetStringValue("linear_solver", "ma27");
        app->Options()->SetStringValue("hessian_approximation", "limited-memory");

        Ipopt::ApplicationReturnStatus status = app->Initialize();
        status = app->OptimizeTNLP(mynlp);

        boost::chrono::time_point<boost::chrono::high_resolution_clock>
                current = boost::chrono::high_resolution_clock::now();

        double elapsed = boost::chrono::nanoseconds(current - start).count() / 1.0e6;
        std::cout << n << ", " << elapsed << std::endl;

//        std::cout << std::setw(widths[0]) << std::left << "Ipopt (analytic)"
//                  << std::fixed << std::setprecision(6)
//                  << std::setw(widths[1]) << std::left << boost::chrono::nanoseconds(current - start).count() / 1.0e9
//                  << std::setw(widths[2]) << std::left << mynlp->feval()
//                  << std::setw(widths[3]) << std::left << mynlp->fcount()
//                  << std::setw(widths[4]) << std::left << min(mynlp->xValue(), n)
//                  << std::setw(widths[5]) << std::left << max(mynlp->xValue(), n)
//                  << std::setw(widths[6]) << std::left << sum(mynlp->xValue(), n)
//                  << std::endl;
    }



    return 0;
}