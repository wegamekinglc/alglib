% add library folder to matlab search path
addpath ./lib

msizes = linspace(100, 2800, 28);
running_times = zeros(length(msizes), 3);

filePath = 'data/20160303_2817.csv';
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
    
    % optimizing with portfolioOptimizer

    tic;
    [cost1, target1] = portfolioOptimizer(var, r, t, w, bndl, bndu, lc, lct);
    toc;
    running_times(i, 1) = toc;
    
    % optimizing with portfolioOptimizerWithTradingCostBudget
    
    tic;
    [cost2, target2] = portfolioOptimizerWithTradingCostBudget(var, r, t, w, 10.0, bndl, bndu, lc, lct);
    toc;
    
    running_times(i, 2) = toc;
    
    % optimizing matlab fmincon
    calc = CostCalculator(var, t, r, w);
    [lb, ub, Aeq, beq] = createConstraints(m);
    A = [];
    b = [];
    nonlcon = [];
    guess = ones(m, 1) / m;

    options = optimoptions('fmincon','MaxFunEvals', 300000, 'GradObj','on', 'Algorithm', 'sqp');
    tic;
    [target3, cost3, exitflag_grad, output_grad] = fmincon(@calc.calculationWithGrad, guess, A, b, Aeq, beq, lb, ub, nonlcon, options);
    toc;
    running_times(i, 3) = toc;
    
end 

semilogy(msizes, running_times);

hold on
xlabel('problem size');
ylabel('time (s)');
legend({'portfolioOptimizer', 'portfolioOptimizerWithTradingCostBudget', 'matlab fmincon'}, 'FontSize', 10, 'Location', 'northoutside','Orientation','horizontal');
hold off