function [lb, ub, eq, eqb] = createConstraints(problemSize)
    m = problemSize;
    
    % 0<= x_i <= 1
    lb = zeros(m, 1);
    ub = ones(m, 1);
    
    % \sum_i x_i = 1
    eq = ones(1, m);
    eqb = 1.;
end

