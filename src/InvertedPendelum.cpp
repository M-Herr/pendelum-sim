#include "InvertedPendelum.hpp"

#include <cmath>
#include <iostream>
#include <algorithm>
#include <eigen3/Eigen/Eigen>


InvertedPendelum::InvertedPendelum(const PendelumOptions& options)
: mMass(options.mass), 
mLength(options.length), 
mGravityAccel(options.gravity_accel), 
mAngle(options.initial_angle),
mAngularVelocity(options.initial_angular_velocity),
mUnwrappedAngle(options.initial_angle)
{
    mMomentOfInertia = mMass*std::pow(mLength, 2);
    this->mState = Eigen::VectorXd(2);
    this->mState << mAngle, mAngularVelocity;
}


InvertedPendelum::InvertedPendelum(const InvertedPendelum& other)
{
    if(this != &other)
    {
        this->mAngle = other.mAngle;
        this->mAngularVelocity = other.mAngularVelocity;
        this->mAngularAcceleration = other.mAngularAcceleration;
        this->mGravityAccel = other.mGravityAccel;
        this->mMass = other.mMass;
        this->mLength = other.mLength;
        this->mUnwrappedAngle = other.mUnwrappedAngle;
        this->mState = other.mState;
    }
}

auto InvertedPendelum::operator=(const InvertedPendelum& other) -> InvertedPendelum& 
{
    if(this != &other)
    {
        this->mAngle = other.mAngle;
        this->mAngularVelocity = other.mAngularVelocity;
        this->mAngularAcceleration = other.mAngularAcceleration;
        this->mGravityAccel = other.mGravityAccel;
        this->mMass = other.mMass;
        this->mLength = other.mLength;
        this->mUnwrappedAngle = other.mUnwrappedAngle;
        this->mState = other.mState;
   }

    return *this;
}

InvertedPendelum::InvertedPendelum(InvertedPendelum&& other) noexcept
{
    if(this != &other)
    {
        this->mAngle = other.mAngle;
        this->mAngularAcceleration = other.mAngularAcceleration;
        this->mAngularVelocity = other.mAngularVelocity;
        this->mGravityAccel = other.mGravityAccel;
        this->mMass = other.mMass;
        this->mLength = other.mLength;
        this->mUnwrappedAngle = other.mUnwrappedAngle;
        this->mState = std::move(other.mState);
    }
}

auto InvertedPendelum::operator=(InvertedPendelum&& other) noexcept -> InvertedPendelum&
{
    if(this != &other)
    {
        this->mAngle = other.mAngle;
        this->mAngularAcceleration = other.mAngularAcceleration;
        this->mAngularVelocity = other.mAngularVelocity;
        this->mGravityAccel = other.mGravityAccel;
        this->mMass = other.mMass;
        this->mLength = other.mLength;
        this->mUnwrappedAngle = other.mUnwrappedAngle;
        this->mState = std::move(other.mState);
    }

    return *this;
}


void InvertedPendelum::CollectTorque(double torque)
{
    mTorque = torque;
}

void InvertedPendelum::Update(double deltaTime)
{

}


Eigen::VectorXd InvertedPendelum::ComputeDeriv(double time, Eigen::VectorXd& state)
{
    Eigen::VectorXd derivs(2);

    //computing dx1/dt and dx2/dt 
    //d\theta^2/dt^2 + w^2sin(\theta) = 0
    double theta = state(0);
    double omega = state(1);

    derivs(0) = std::min(omega,3.0);

    derivs(1) = -(mGravityAccel/mLength)*std::sin(theta) + mTorque/(mMass*mLength*mLength);

    return derivs;
}

void InvertedPendelum::ApplyIntegration()
{
    mAngle = std::fmod(mState(0) + M_PI, 2.0 * M_PI) - M_PI;
    mAngularVelocity = mState(1);
}