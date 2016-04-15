function [cost, target] = portfolioOptimizer(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType)
    if ~libisloaded('optimizer')
        loadlibrary('optimizer', 'optimizer.hpp'); % msvc
    end
    
    if libisloaded('optimizer')
        libname = 'optimizer';
    end
    
    [m, var, r, t, w, bndl, bndu, lc, lct, lcm] = argumentChecker(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType);

    target = libpointer('doublePtr', zeros(1, m));
    cost = libpointer('doublePtr', 0.0);
    
    calllib(libname, 'portfolioOptimizer', m, var, r, t, w, bndl, bndu, lcm, lc, lct, target, cost);
    cost = double(cost.Value);
    target = double(target.Value);
end

