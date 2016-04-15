% add library folder to matlab search path
addpath ./lib

m = 500;

filePath = 'data/20160303_500.csv';
varMatrix = csvread(filePath);

[var, t, r, w] = paramaterReader(varMatrix, m);

% 0 <= w_i <= 1.0
bndl = zeros(1, m);
bndu = ones(1, m);


% \sum w_i = 1
lc = ones(1, m + 1);
lct = 0;

% total trading cost buget is 0.0005
eta = 0.0005;

% do the optimization
[cost, target] = portfolioOptimizerWithTradingCostBudget(var, r, t, w, eta, bndl, bndu, lc, lct);

fprintf('%15s %8.4f\n', 'f(w):', cost);
fprintf('%15s %8.4f\n', 'sum(w):', sum(target));
fprintf('%15s %8.4f\n', 'min(w):', min(target));
fprintf('%15s %8.4f\n', 'max(w):', max(target));
fprintf('%15s %8.4f\n', 'trading cost:', abs(target' - w)' * t);