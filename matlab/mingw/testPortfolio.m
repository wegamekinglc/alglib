% problem size
m = 100;
filePath = strcat('../../data/20160303_', int2str(m), '.csv');

[var, t, r, w] = paramaterReader(filePath);

tic;
[cost, target] = portfolioOptimizer(var, r, t, w);
toc;