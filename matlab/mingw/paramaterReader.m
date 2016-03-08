function [varMatrix, tradingCost, expectedReturn, currentWeight] = paramaterReader(filePath)
    varMatrix = csvread(filePath);
    [m, n] = size(varMatrix);
    assert(m == n)
    % m = 2
    varMatrix = varMatrix(1:m, 1:m);
    tradingCost = ones(m, 1) * 0.003;
    expectedReturn = linspace(0, 0.2, m+1)';
    expectedReturn = expectedReturn(1:m, 1);
    currentWeight = ones(m, 1) / m;
end

