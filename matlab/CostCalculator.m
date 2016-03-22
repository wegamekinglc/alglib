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
            
            calc.d_varMatrix_ = gpuArray(varMatrix);
            calc.d_tradingCost_ = gpuArray(tradingCost);
            calc.d_expectedReturn_ = gpuArray(expectedReturn);
            calc.d_currentWeight_ = gpuArray(currentWeight);
        end                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                      
        
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
        
        function [func, grad] = calculationWithGradAndGPU(calc, newWeight)
            d_newWeight = gpuArray(newWeight);
            
            % risk cost
            grad = calc.d_varMatrix_ * d_newWeight;
            func = 0.5 * grad' * d_newWeight;
            
            % trading cost
            weightChange = d_newWeight - calc.d_currentWeight_;
            grad = grad + sign(weightChange) .* calc.d_tradingCost_; 
            func = func + calc.d_tradingCost_' * abs(weightChange);
            
            % expected return contribution
            grad = grad - calc.d_expectedReturn_;
            func = func - d_newWeight' * calc.d_expectedReturn_;
            
            func = gather(func);
            grad = gather(grad);
        end                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                  
    end
end

