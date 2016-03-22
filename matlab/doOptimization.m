% problem size
m = input('Please input size: ');
filePath = strcat('../data/20160303_', int2str(m), '.csv');
fprintf('%f', m)

[varMatrix, tradingCost, expectedReturn, currentWeight] = paramaterReader(filePath);
calc = CostCalculator(varMatrix, tradingCost, expectedReturn, currentWeight);
[lb, ub, Aeq, beq] = createConstraints(m);
A = [];
b = [];
nonlcon = [];
guess = randn(m, 1);

% optimize without gradient
%options = optimoptions('fmincon','MaxFunEvals',300000, 'Algorithm', 'sqp');
%tic;
%[x, fval, exitflag, output] = fmincon(@calc.calculation, guess, A, b, Aeq, beq, lb, ub, nonlcon, options);
%toc;

% optimize with gradient
options = optimoptions('fmincon','MaxFunEvals', 300000, 'GradObj','on', 'Algorithm', 'sqp');
tic;
[x_grad, fval_grad, exitflag_grad, output_grad] = fmincon(@calc.calculationWithGrad, guess, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;

% optimize with gradient and GPU
options = optimoptions('fmincon','MaxFunEvals',300000, 'GradObj','on', 'Algorithm', 'sqp');
tic;
[x_gpu, fval_gpu, exitflag_gpu, output_gpu] = fmincon(@calc.calculationWithGradAndGPU, currentWeight, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;                                                                                                                                                                                                                                                                                              