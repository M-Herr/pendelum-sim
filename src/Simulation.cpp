#include "Simulation.hpp"
#include "Utils.hpp"

#include <algorithm>
#include <chrono>
#include <thread>
#include <limits>

void Simulation::AddJob(std::shared_ptr<FunctionJob> new_job, SimulationJobTypes job_type)
{   

    if(std::abs(new_job->GetJobRate()) <= std::numeric_limits<double>::epsilon())
    {
        std::cerr << "Job added with a job rate of zero: Defaulting rate to 1.0 s\n";
        new_job->SetJobRate(1.0);
    }

    new_job->SetJobRateTics(DoubleToTics(new_job->GetJobRate()));

    switch(job_type)
    {
        case SimulationJobTypes::Scheduled:
        {
            new_job->SetNextRunTics(new_job->GetJobRateTics());
            Utils::EmplaceBackWithCheck(mScheduledJobs, new_job);
            break;
        }
        case SimulationJobTypes::PreIntegrator:
        {
            Utils::EmplaceBackWithCheck(mPreIntegratorJobs, new_job);
            break;
        }
        case SimulationJobTypes::Integrator:
        {
            new_job->SetNextRunTics(new_job->GetJobRateTics());
            Utils::EmplaceBackWithCheck(mIntegratorJobs, new_job);
            break;
        }
        case SimulationJobTypes::PostIntegrator:
        {
            Utils::EmplaceBackWithCheck(mPostIntegratorJobs, new_job);
            break;
        }
        default:
        {
            break;
        }
    }    
}

void Simulation::Run()
{
    size_t this_time_step_tics = 0;
    

    

    while(run.load())
    {   
        if(!pause.load())
        {
            std::vector<std::shared_ptr<FunctionJob>> jobs_this_frame;
            jobs_this_frame.reserve(mScheduledJobs.size() 
                                    + mIntegratorJobs.size() 
                                    + mPostIntegratorJobs.size());

            for(auto scheduled_job : mScheduledJobs)
            {
                if(scheduled_job->GetNextRunTics() <= (mTime + mTimeStepTics))
                {
                    //Needs to run before the next 'big' time step
                    jobs_this_frame.emplace_back(scheduled_job);
                }
            }

            for(auto integration_job : mIntegratorJobs)
            {
                if(integration_job->GetNextRunTics() <= (mTime + mTimeStepTics))
                {
                    jobs_this_frame.emplace_back(integration_job);
                }
            }

            for(auto post_integration_job : mPostIntegratorJobs)
            {
                if(post_integration_job->GetNextRunTics() <= (mTime + mTimeStepTics))
                {
                    jobs_this_frame.emplace_back(post_integration_job);
                }
            }
            auto start = std::chrono::high_resolution_clock::now();

            size_t next_time = mTime + mTimeStepTics;

            while(mTime < next_time)
            {
                auto next_scheduled = FindNextScheduledTic(jobs_this_frame);
                if(next_scheduled.second)
                {
                    mTime = next_scheduled.first;

                    next_scheduled.second->Execute(next_scheduled.second->GetJobRate());
                    next_scheduled.second->SetNextRunTics(mTime + next_scheduled.second->GetJobRateTics());
                    next_scheduled.second->SetLastRunTics(mTime);
                    
                    for(auto job : jobs_this_frame)
                    {
                        if(job != next_scheduled.second)
                        {
                            if(job->GetNextRunTics() == mTime)
                            {
                                //std::cout << "Running job: " << job->GetName() << " \n";
                                //std::cout << "Current Time: " << TicsToDouble(mTime) << "\n";
                                //std::cout << "Previous Run Time: " << TicsToDouble(job->GetLastRunTics()) << "\n";
                                job->Execute(job->GetJobRate());
                                job->SetNextRunTics(mTime + job->GetJobRateTics());
                                job->SetLastRunTics(mTime);
                            }
                        }
                        
                    }
                }
                else 
                {
                    mTime += mTimeStepTics;
                }

            }
            
            double deltaTime = static_cast<double>(mTimeStepTics) / static_cast<double>(mTicsPerSecond);
        
            // Calculate the actual sleep time to match the scaled time
            auto end = std::chrono::high_resolution_clock::now();
            auto elapsed = std::chrono::duration<double>(end - start).count();
            double sleep_time = (static_cast<double>(mTimeStepTics) / (mTicsPerSecond * mTimeScale)) - elapsed;
            if (sleep_time > 0) {
                std::this_thread::sleep_for(std::chrono::duration<double>(sleep_time));
            }
            end = std::chrono::high_resolution_clock::now();
            
            double loop_duration = std::chrono::duration<double>(end - start).count();
            
            mMeasuredTimeScale = deltaTime/loop_duration;
            mLastStepTics = mTime;

            std::cout << mMeasuredTimeScale << std::endl;
        }

    }

    //jobs_this_frame.clear();

}


void Simulation::Stop()
{
    run.store(false);
}

void Simulation::Pause()
{
   
}
auto Simulation::FindNextScheduledTic(std::vector<std::shared_ptr<FunctionJob>>& jobs_to_check) -> std::pair<size_t, std::shared_ptr<FunctionJob>>
{
    std::shared_ptr<FunctionJob> earliest_job = nullptr;
    size_t earliest_time = std::numeric_limits<size_t>::max(); 

    size_t endOfBigStep = mTime + mTimeStepTics;

    for (auto& job : jobs_to_check)
    {
        size_t jobTime = job->GetNextRunTics();

        // We only care if jobTime is strictly after mTime 
        // but before the end of our large step
        if (jobTime > mTime && jobTime <= endOfBigStep) 
        {
            if (jobTime < earliest_time)
            {
                earliest_time = jobTime;
                earliest_job  = job;
            }
        }
    }

    return { earliest_time, earliest_job };
}