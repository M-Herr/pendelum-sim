#include "SimulationJob.hpp"
#include <iostream>

SimulationJob::SimulationJob(const std::string& job_name)
: mName(job_name)
{

}

void SimulationJob::AddRunBeforeJob(std::pair<std::string, std::shared_ptr<SimulationJob>> run_before)
{
    mRunBeforeJobs[run_before.first].push_back(run_before.second);
}

void SimulationJob::AddRunAfterJob(std::pair<std::string, std::shared_ptr<SimulationJob>> run_after)
{
    mRunAfterJobs[run_after.first].push_back(run_after.second);
}

void SimulationJob::AddDependency(std::pair<std::string, std::shared_ptr<SimulationJob>> dependency)
{
    mDependsOnJobs[dependency.first].push_back(dependency.second);
}

bool SimulationJob::CanRun() const
{
    for (const auto& [key, jobs] : mDependsOnJobs) {
        for (const auto& job : jobs) {
            if (!job->IsComplete()) {
                return false;
            }
        }
    }
    return true;   
}

void SimulationJob::RunJob(double deltaTime)
{
   //std::cout << "BNase class \"Run Job\" function.\n"; 
}  

void SimulationJob::Execute(double deltaTime) 
{
    if (!CanRun()) {
        std::cerr << "Dependencies not satisfied for job: " << mName << std::endl;
        return;
    }
   //std::cout << "Executing job: " << mName << std::endl;
    RunJob(deltaTime);
    mComplete = true;
    for (const auto& [key, jobs] : mRunAfterJobs) {
        for (const auto& job : jobs) {
            if (job->CanRun()) {
                job->Execute(deltaTime);
            }
        }
    }
}


bool SimulationJob::IsComplete() const
{
    return mComplete;
}