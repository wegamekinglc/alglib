#include <stdio.h>
#include <windows.h>
#include "LinAlg.h"

double counter()
{
    return 0.001*GetTickCount();
}

int main()
{
    alglib::real_2d_array a, b, c;
    int n = 2000;
    int i, j;
    double timeneeded, flops;
    
    // Initialize arrays
    a.setlength(n, n);
    b.setlength(n, n);
    c.setlength(n, n);
    for(i=0; i<n; i++)
        for(j=0; j<n; j++)
        {
            a[i][j] = alglib::randomreal()-0.5;
            b[i][j] = alglib::randomreal()-0.5;
            c[i][j] = 0.0;
        }
    
    // Set number of worker threads: "4" means "use 4 cores".
    // This line is ignored if AE_OS is UNDEFINED.
    alglib::setnworkers(4);
    
    // Perform matrix-matrix product.
    // We call function with "smp_" prefix, which means that ALGLIB
    // will try to execute it in parallel manner whenever it is possible.
    flops = 2*pow((double)n, (double)3);
    timeneeded = counter();
    alglib::smp_rmatrixgemm(
        n, n, n,
        1.0,
        a, 0, 0, 0,
        b, 0, 0, 1,
        0.0,
        c, 0, 0);
    timeneeded = counter()-timeneeded;
    
    // Evaluate performance
    printf("Performance is %.1f GFLOPS\n", (double)(1.0E-9*flops/timeneeded));
    
    return 0;
}
