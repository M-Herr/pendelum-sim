#include "RungeKutta4.hpp"

#include <iostream>
#include <algorithm>
#include <type_traits>

RungeKutta4Integrator::RungeKutta4Integrator()
{

}

RungeKutta4Integrator::~RungeKutta4Integrator()
{

}

void RungeKutta4Integrator::Integrate(double deltaTime)
{
    //std::cout << "Begin state:\n" << state << "\n";
    for(size_t i = 0; i < mDerivs.size(); i++)
    {
        DerivFuncType deriv = mDerivs[i];
        StateType& state = *mStates[i];

        double halfDt = 0.5*deltaTime;

        double t = mCurrentTime;

        
        StateType K1 = std::invoke(deriv, t, state);

        StateType K2_state_args = state + K1*halfDt;
        StateType K2 = std::invoke(deriv, t + halfDt, K2_state_args);

        StateType K3_state_args = state + K2*halfDt;
        StateType K3 = std::invoke(deriv, t + halfDt, K3_state_args);

        StateType K4_state_args = K3*deltaTime;
        StateType K4 = std::invoke(deriv, t + deltaTime, K4_state_args);

        // Update the state
        state = state + (K1 + K2*2.0 + K3*2.0 + K4) * (deltaTime / 6.0);

        
    }

    // Update the time
    mCurrentTime += deltaTime;
    //std::cout << "End state:\n" << state << "\n";
}