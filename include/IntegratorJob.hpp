#pragma once
#include <memory>

#include <eigen3/Eigen/Eigen>


#include "IntegratorBase.hpp"


//Fwd Declares
class RungeKutta4Integrator;


enum class IntegratorType 
{
    RungeKutta4,
};

class IntegratorJob
{
    public:
    IntegratorJob() = default;
    ~IntegratorJob() = default;


    void CreateIntegrator(IntegratorType integrator_type);
    
    private:
    std::shared_ptr<IntegratorBase> mIntegrator;
    IntegratorType mIntegratorType;

};