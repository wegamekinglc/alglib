% only used for performance comparision
classdef CostCalculator
    
    properties
        varMatrix_;
        currentWeight_;
        expectedReturn_;
        tradingCost_;
        
        d_varMatrix_;
        d_currentWeight_;
        d_expectedReturn_;
        d_tradingCost_;
        
    end
  
    methods
        function calc = CostCalculator(varMatrix, tradingCost, expectedReturn, currentWeight)
            calc.varMatrix_ = varMatrix;
            calc.tradingCost_ = tradingCost;
            calc.expectedReturn_ = expectedReturn;
            calc.currentWeight_ = currentWeight;
           
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

