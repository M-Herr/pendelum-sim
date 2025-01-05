#pragma once
#include "IntegratorBase.hpp"


class RungeKutta4Integrator : public IntegratorBase
{
    public:

    RungeKutta4Integrator();
    ~RungeKutta4Integrator();


    virtual void Integrate(double deltaTime) override;
    
    void SetDerivFunction(); 
    void SetTimeStep();
    
    private:
    
    double mTimeStep{0.0};
    double mCurrentTime{0.0};
};