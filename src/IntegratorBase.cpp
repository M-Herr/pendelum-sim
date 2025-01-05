#include "IntegratorBase.hpp"

void IntegratorBase::AddDerivativeFunction(DerivFuncType derivative_function, StateType& state_ref)
{
    mDerivs.emplace_back(derivative_function);
    mStates.emplace_back(&state_ref);
}