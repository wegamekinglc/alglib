function [cost, target] = portfolioOptimizer(covMatrix, expRet, tCost, weight, lowerBound, upperBound, linearCond, linearCondType)
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
    
    [m, n] = size(covMatrix);
    
    assert(m == n)
    [rm, rn] = size(expRet);
    assert((rm==m && rn==1) || (rm==1 && rn==m))
    
    [tm, tn] = size(tCost);
    assert((tm==m && tn==1) || (tm==1 && tn==m))
    
    [wm, wn] = size(weight);
    assert((wm==m && wn==1) || (wm==1 && wn==m))
    
    % bounded constraints settings
    if nargin <= 4
        bndl = libpointer('doublePtr');
        bndu = libpointer('doublePtr');
    else
        [blm, bln] = size(lowerBound);
        assert((blm==m && bln==1) || (blm==1 && bln==m))
        [bum, bun] = size(upperBound);
        assert((bum==m && bun==1) || (bum==1 && bun==m))
        bndl = libpointer('doublePtr', lowerBound);
        bndu = libpointer('doublePtr', upperBound); 
    end
    
    % linear equality/inequality constraints
    if nargin <= 6
        lc = libpointer('doublePtr');
        lct = libpointer('int32Ptr');
        lcm = 0;
    else
        [lcm, lcn] = size(linearCond);
        assert(lcn == m + 1)
        [lctm, lctn] = size(linearCondType);
        assert((lctm==lcm && tn==1) || (lctm==1 && lctn==lcm))
        lc = libpointer('doublePtr', linearCond);
        lct = libpointer('int32Ptr', linearCondType);
    end
    
    % transfrom matlab data type to corresponding c pointer type
    var =  libpointer('doublePtr', covMatrix);
    r = libpointer('doublePtr', expRet);
    t = libpointer('doublePtr', tCost);
    w = libpointer('doublePtr', weight);
    
    target = libpointer('doublePtr', zeros(1, m));
    cost = libpointer('doublePtr', 0.0);
    
    calllib(libname, 'portfolioOptimizer', m, var, r, t, w, bndl, bndu, lcm, lc, lct, target, cost);
    cost = double(cost.Value);
    target = double(target.Value);
end

