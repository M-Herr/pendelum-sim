#pragma once

#include <functional>
#include <iostream>
#include <type_traits>

#include "SimulationJob.hpp"

class FunctionJob : public SimulationJob {
public:
    explicit FunctionJob(const std::string& job_name, std::function<void(double)> func)
        : SimulationJob(job_name), mFunction(std::move(func)) {}

    

protected:
    virtual void RunJob(double deltaTime) override 
    {
        if (mFunction) {
            std::invoke(mFunction, deltaTime);

        } else {
            std::cerr << "FunctionJob: No function to execute for job: " << GetName() << std::endl;
        }
    }

    

private:
    std::function<void(double)> mFunction;
    
};

