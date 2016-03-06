% problem size
m = 500;
filePath = strcat('../data/20160303_', int2str(m), '.csv');

[varMatrix, tradingCost, expectedReturn, currentWeight] = paramaterReader(filePath);
calc = CostCalculator(varMatrix, tradingCost, expectedReturn, currentWeight);
[lb, ub, Aeq, beq] = createConstraints(m);
A = [];
b = [];
nonlcon = [];

% optimize without gradient
tic;
options = optimoptions('fmincon','MaxFunEvals',300000);
[x, fval, exitflag, output] = fmincon(@calc.calculation, currentWeight, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;

% optimize  with gradient
tic;
options = optimoptions('fmincon','MaxFunEvals',300000, 'GradObj','on');
[x_grad, fval_grad, exitflag_grad, output_grad] = fmincon(@calc.calculationWithGrad, currentWeight, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;