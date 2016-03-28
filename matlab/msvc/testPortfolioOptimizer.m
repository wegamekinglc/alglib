% problem size
m = 500;
filePath = strcat('../../data/20160303_', int2str(m), '.csv');

[var, t, r, w] = paramaterReader(filePath);

bndl = zeros(1, m);
bndu = ones(1, m);

lc = ones(1, m + 1);
lct = 0;

epsg = 1e-8;
epsf = 1e-8;
epsx = 1e-8;
maxits = 0;

setStopCondition(epsg, epsf, epsx, maxits);

tic;
[cost, target] = portfolioOptimizer(var, r, t, w, bndl, bndu, lc, lct);
toc;

tic;
[costCuda, targetCuda] = portfolioOptimizerCuda(var, r, t, w, bndl, bndu, lc, lct);
toc;