% add library folder to matlab search path
addpath ./lib

m = 500;

filePath = 'data/20160303_500.csv';
varMatrix = csvread(filePath);

[var, t, r, w] = paramaterReader(varMatrix, m);

% 0 <= w_i <= 0.2
bndl = zeros(1, m);
bndu = ones(1, m) * 0.2;


% 0.25 < \sum w_i < 0.75
lc = ones(2, m + 1);
lc(1, m + 1) = 0.75;
lc(2, m + 1) = 0.25;
lct = [-1, 1];

% do the optimization
[cost, target] = portfolioOptimizer(var, r, t, w, bndl, bndu, lc, lct);

fprintf('%15s %8.4f\n', 'f(w):', cost);
fprintf('%15s %8.4f\n', 'sum(w):', sum(target));
fprintf('%15s %8.4f\n', 'min(w):', min(target));
fprintf('%15s %8.4f\n', 'max(w):', max(target));
fprintf('%15s %8.4f\n', 'trading cost:', abs(target' - w)' * t);