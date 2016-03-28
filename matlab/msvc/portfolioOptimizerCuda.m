function [cost, target] = portfolioOptimizerCuda(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType)
    if ~libisloaded('optimizer') && ~libisloaded('liboptimizer')
        try
            loadlibrary('optimizer', 'optimizer.hpp'); % msvc
        catch
            loadlibrary('liboptimizer', 'optimizer.hpp'); % mingw
        end
    end
    
    if libisloaded('optimizer')
        libname = 'optimizer';
    elseif libisloaded('liboptimizer')
        libname = 'liboptimizer';   
    end
    
    [m, var, r, t, w, bndl, bndu, lc, lct, lcm] = argumentChecker(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType);
    
    target = libpointer('doublePtr', zeros(1, m));
    cost = libpointer('doublePtr', 0.0);
    
    calllib(libname, 'portfolioOptimizerCuda', m, var, r, t, w, bndl, bndu, lcm, lc, lct, target, cost);
    cost = double(cost.Value);
    target = double(target.Value);
end

