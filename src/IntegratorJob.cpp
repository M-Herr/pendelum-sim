#include "IntegratorJob.hpp"

#include "RungeKutta4.hpp"
#include <iostream>


void IntegratorJob::CreateIntegrator(IntegratorType integrator_type)
{
    switch(integrator_type)
    {
        case IntegratorType::RungeKutta4:
        {
            mIntegrator = std::make_unique<RungeKutta4Integrator>();
            break;
        }
        default:
        {
            std::cerr << "Failed to create an integrator for some reason.\n";
            break;
        }
    }
}