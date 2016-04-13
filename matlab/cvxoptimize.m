filePath = strcat('../data/20160303_500.csv');
varMatrix = csvread(filePath);

m = 500;

[var, tradingCost, expectedReturn, currentWeight] = paramaterReader(varMatrix, m);

guess = ones(m, 1) / m;
lb = zeros(m, 1);
ub = ones(m, 1);

C = ones(1, m);
d = 1;

cost = 0.5 * guess' * var * guess - expectedReturn' * guess;
w = warning ('off','MATLAB:nargchk:deprecated');

cvx_quiet true
cvx_solver SeDuMi
cvx_precision high

cvx_begin
    variable guess(m)
    minimize(0.5 * guess' * var * guess - expectedReturn' * guess)
    subject to
        lb <= guess <= ub;
        C * guess == d;
        norm((guess - currentWeight) .* tradingCost,1) <= 0.0005
cvx_end