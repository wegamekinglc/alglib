classdef CostCalculator
    
    properties
        varMatrix_;
        currentWeight_;
        expectedReturn_;
        tradingCost_;
    end
  
    methods
        function calc = CostCalculator(varMatrix, tradingCost, expectedReturn, currentWeight)
            calc.varMatrix_ = varMatrix;
            calc.tradingCost_ = tradingCost;
            calc.expectedReturn_ = expectedReturn;
            calc.currentWeight_ = currentWeight;
        end
        
        function func = calculation(calc, newWeight)
            % risk cost
            func = 0.5 * newWeight' * calc.varMatrix_ * newWeight;
            
            % trading cost
            func = func + calc.tradingCost_' * abs(newWeight - calc.currentWeight_);
            
            % expected return contribution
            func = func - newWeight' * calc.expectedReturn_;
        end
        
        function [func, grad] = calculationWithGrad(calc, newWeight)
            % risk cost
            grad = calc.varMatrix_ * newWeight;
            func = 0.5 * grad' * newWeight;
            
            % trading cost
            weightChange = newWeight - calc.currentWeight_;
            grad = grad + sign(weightChange) .* calc.tradingCost_; 
            func = func + calc.tradingCost_' * abs(weightChange);
            
            % expected return contribution
            grad = grad - calc.expectedReturn_;
            func = func - newWeight' * calc.expectedReturn_;
        end
    end
end

