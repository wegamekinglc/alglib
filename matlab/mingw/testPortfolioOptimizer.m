% problem size
m = 100;
filePath = strcat('../../data/20160303_', int2str(m), '.csv');

[var, t, r, w] = paramaterReader(filePath);

bndl = zeros(1, m);
bndu = ones(1, m);

lc = ones(1, m + 1);
lct = zeros(1,1);


tic;
[cost, target] = portfolioOptimizer(var, r, t, w, bndl, bndu, lc, lct);
toc;