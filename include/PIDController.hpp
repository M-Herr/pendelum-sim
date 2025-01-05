#pragma once
#include "PhysicsObject.hpp"


class PIDController
{
    public:
    PIDController();

    PIDController(double Kp, double Ki, double Kd);

    ~PIDController();

    auto GetOutput(double setpoint, double actual, double deltaTime) -> double;

    void Update(double setpoint, double actual, double deltaTime);

    void SetGains(double dKp, double dKi, double dKd);

    double GetError() { return mError; }
    double GetErrorDerivative() { return mErrorDerivative; }
    double GetErrorIntegral() { return mErrorIntegral; }
    
    double GetKp() const { return mKp;} 
    double GetKi() const { return mKi;}
    double GetKd() const { return mKd;}

    private:
    double mKp{1.0};
    double mKi{1.0};
    double mKd{1.0};

    double mLastError{0.0};
    
    double mError{0.0};
    double mErrorDerivative{0.0};
    double mErrorIntegral{0.0};
};