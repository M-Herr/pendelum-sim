#pragma once

#include <vector>
#include <functional>


#include <eigen3/Eigen/Eigen>

class IntegratorBase 
{
    public:
    using StateType = Eigen::VectorXd;
    using DerivFuncType = std::function<StateType(double, StateType&)>;

    virtual ~IntegratorBase() = default;


    virtual void Integrate(double deltaTime) = 0;
    
    virtual void AddDerivativeFunction(DerivFuncType derivative_function, StateType& state_ref);

    
    protected:
    std::vector<DerivFuncType> mDerivs;
    std::vector<StateType*> mStates;

};