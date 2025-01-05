#pragma once

#include "PhysicsObject.hpp"
#include <eigen3/Eigen/Eigen>


struct PendelumOptions
{
    double mass{0.0};
    double length{0.0};
    double gravity_accel{0.0};
    double initial_angle{0.0};
    double initial_angular_velocity{0.0};
};


class InvertedPendelum : public PhysicsObject
{
    public:
    InvertedPendelum() = default;
    explicit InvertedPendelum(const PendelumOptions& options);
    ~InvertedPendelum() = default;

    InvertedPendelum(const InvertedPendelum& other);

    auto operator=(const InvertedPendelum& other) -> InvertedPendelum&;

    InvertedPendelum(InvertedPendelum&& other) noexcept;

    auto operator=(InvertedPendelum&& other) noexcept -> InvertedPendelum&;

    auto GetLength() const -> double;
    auto GetMass() const -> double;
    auto GetGravityAccel() const -> double;
    auto GetAngle() const -> double;
    auto GetAngularVelocity() const -> double;
    inline auto GetTorque() const -> double;

    void CollectTorque(double torque) ;
    
    virtual void Update(double deltaTime) override;

    Eigen::VectorXd ComputeDeriv(double time, Eigen::VectorXd& state);
    void ApplyIntegration();
    
    inline Eigen::VectorXd& GetStateRef();
    
    

    private:
    
    double mMass{0.0};
    double mLength{0.0};
    double mGravityAccel{0.0};
    double mAngle{0.0};
    double mAngularVelocity{0.0};
    double mAngularAcceleration{0.0};
    double mTorque{0.0};
    double mUnwrappedAngle{0.0};
    double mDamping{2.0};
    double mMomentOfInertia{0.0};

    Eigen::VectorXd mState;
};

inline auto InvertedPendelum::GetLength() const -> double
{
    return mLength;
}

inline auto InvertedPendelum::GetMass() const -> double
{
    return mMass;
}

inline auto InvertedPendelum::GetGravityAccel() const -> double
{
    return mGravityAccel;
}

inline auto InvertedPendelum::GetAngle() const -> double
{
    return mAngle;
}

inline  auto InvertedPendelum::GetAngularVelocity() const -> double
{
    return mAngularVelocity;
}

inline Eigen::VectorXd& InvertedPendelum::GetStateRef() 
{ 
    return mState; 
}

inline auto InvertedPendelum::GetTorque() const -> double
{
    return mTorque;
}