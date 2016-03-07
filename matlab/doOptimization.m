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
options = optimoptions('fmincon','MaxFunEvals',300000, 'Algorithm', 'sqp');
tic;
[x, fval, exitflag, output] = fmincon(@calc.calculation, currentWeight, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;

% optimize  with gradient
options = optimoptions('fmincon','MaxFunEvals',300000, 'GradObj','on', 'Algorithm', 'sqp');
tic;
[x_grad, fval_grad, exitflag_grad, output_grad] = fmincon(@calc.calculationWithGrad, currentWeight, A, b, Aeq, beq, lb, ub, nonlcon, options);
toc;