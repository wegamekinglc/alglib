function [varMatrix, tradingCost, expectedReturn, currentWeight] = paramaterReader(varMatrixOrig, tm)
    [m, n] = size(varMatrixOrig);
    assert(m == n)
    
    varMatrix = varMatrixOrig(1:tm, 1:tm);
    tradingCost = ones(tm, 1) * 0.003;
    expectedReturn = linspace(0, 0.2, tm+1)';
    expectedReturn = expectedReturn(1:tm, 1);
    currentWeight = ones(tm, 1) / tm;
end

