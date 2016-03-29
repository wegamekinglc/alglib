% problem size
msizes = [100, 200, 300, 400, 500, 700, 1000, 1500, 2000];
alglib_times = zeros(length(msizes), 2);

filePath = strcat('../../data/20160303_2000.csv');
varMatrix = csvread(filePath);

for i = 1:length(msizes)
    m = msizes(i);
    
    [var, t, r, w] = paramaterReader(varMatrix, m);

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
    alglib_times(i, 1) = toc;

    tic;
    [costCuda, targetCuda] = portfolioOptimizerCuda(var, r, t, w, bndl, bndu, lc, lct);
    toc;
    alglib_times(i, 2) = toc;
    
end 