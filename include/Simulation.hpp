#pragma once
#include <vector>
#include <memory>
#include <atomic>
#include <cmath>
#include <iostream>
#include <map>


#include "FunctionJob.hpp"
#include "Time.hpp"

enum class SimulationJobTypes
{
    Scheduled,
    PreIntegrator,
    Integrator,
    PostIntegrator,
};

class Simulation
{
    public:
    Simulation() = default;
    ~Simulation() = default;

    void AddJob(std::shared_ptr<FunctionJob> new_object, SimulationJobTypes job_type);

    void Run();
    

    inline void SetTicsPerSecond(size_t tics); 
    inline size_t GetTicsPerSecond() const;

    inline size_t GetTimeStepTics() const;
    inline void SetTimeStepTics(size_t tics);

    inline void SetTimeStep(double time_step);
    
    inline double GetTimeScale() const;
    inline void SetTimeScale(double time_scale);
    
    inline size_t GetTimeTics() const;
    
    inline double GetTime() const;

    inline void SetRealTime(bool real_time);

    void Stop();
    void Pause();

    
    private:

    std::atomic<bool> run{true};
    std::atomic<bool> pause{false};

    std::vector<std::shared_ptr<FunctionJob>> mScheduledJobs;
    std::vector<std::shared_ptr<FunctionJob>> mPreIntegratorJobs;
    std::vector<std::shared_ptr<FunctionJob>> mIntegratorJobs;
    std::vector<std::shared_ptr<FunctionJob>> mPostIntegratorJobs;

    size_t mTicsPerSecond{100000000};
    size_t mTimeStepTics{100000}; 
    size_t mLastStepTics{0};
    size_t mTime{0};
    double mTimeScale{1.0};
    bool mRealTime{false};
    double mMeasuredTimeScale{1.0};
    private:

    inline size_t DoubleToTics(double time) const;
    inline double TicsToDouble(size_t tics) const;

    auto FindNextScheduledTic(std::vector<std::shared_ptr<FunctionJob>>& jobs_to_check) -> std::pair<size_t, std::shared_ptr<FunctionJob>>;
};

inline size_t Simulation::GetTicsPerSecond() const
{
    return mTicsPerSecond;
}

inline void Simulation::SetTicsPerSecond(size_t tics)
{
    mTicsPerSecond = tics;
}

inline size_t Simulation::GetTimeStepTics() const
{
    return mTimeStepTics;
}

inline void Simulation::SetTimeStepTics(size_t tics)
{
    mTimeStepTics = tics;
}

inline void Simulation::SetTimeStep(double time_step)
{
    //deltaTime = double(1/mTicsPerSecond)
    //
    mTimeStepTics = static_cast<size_t>(std::round(time_step * mTicsPerSecond));
}

inline void Simulation::SetTimeScale(double time_scale)
{
    if(time_scale > 0)
    {
        mTimeScale = time_scale;
    }
    else 
    {
        std::cerr << "Time scale must be positive and larger than zero.\n";
    }
}

inline double Simulation::GetTimeScale() const
{
    return mTimeScale;
}

inline size_t Simulation::GetTimeTics() const
{
    return mTime;
}
    
inline double Simulation::GetTime() const
{
    return TicsToDouble(mTime);
}

inline size_t Simulation::DoubleToTics(double time) const 
{
    return static_cast<size_t>(time*static_cast<double>(mTicsPerSecond));
}

inline double Simulation::TicsToDouble(size_t tics) const 
{
    return static_cast<double>(tics)/static_cast<double>(mTicsPerSecond);
}


inline void Simulation::SetRealTime(bool real_time)
{
    mRealTime = real_time;
}