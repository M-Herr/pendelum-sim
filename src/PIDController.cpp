#include "PIDController.hpp"
#include <cmath>
#include <algorithm>

 PIDController::PIDController()
 {

 }

PIDController::PIDController(double Kp, double Ki, double Kd)
: mKp(Kp), mKi(Ki), mKd(Kd)
{

}

PIDController::~PIDController()
{

}

double PIDController::GetOutput(double setpoint, double actual, double deltaTime)
{

    return mKp*mError + mKi*mErrorIntegral + mKd*mErrorDerivative;
}

void PIDController::Update(double setpoint, double actual, double deltaTime)
{
    mError = setpoint - actual;
    
    // Wrap the difference into [-180, 180)
    while (mError <= -M_PI) mError += 2.0*M_PI;
    while (mError >  M_PI)  mError -= 2.0*M_PI;
        
    mErrorDerivative = (mError - mLastError)/deltaTime;

    if(std::abs(mErrorIntegral) >= 100)
    {
        mErrorIntegral = 0.0;
    }

    mLastError = mError;
}   

void PIDController::SetGains(double dKp, double dKi, double dKd)
{
    mKp = std::clamp(0.0, mKp + dKp, 500.0);
    mKi = std::clamp(0.0, mKi + dKi, 500.0);
    mKd = std::clamp(0.0, mKd + dKd, 500.0);
}