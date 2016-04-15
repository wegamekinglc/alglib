function [cost, target] = portfolioOptimizerWithTradingCostBudget(covMatrix, expRet, tCost, weight, tCostBuget, lowerBound, upperBound, linearCond, linearCondType)
    if ~libisloaded('optimizer')
        loadlibrary('optimizer', 'optimizer.hpp'); % msvc
    end
    
    if libisloaded('optimizer')
        libname = 'optimizer';
    end
    
    [m, var, r, t, w, bndl, bndu, lc, lct, lcm] = argumentChecker(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType);

    target = libpointer('doublePtr', zeros(1, m));
    cost = libpointer('doublePtr', 0.0);
    
    calllib(libname, 'portfolioOptimizerWithTradingCostBudget', m, var, r, t, w, tCostBuget, bndl, bndu, lcm, lc, lct, target, cost);
    cost = double(cost.Value);
    target = double(target.Value);
end


