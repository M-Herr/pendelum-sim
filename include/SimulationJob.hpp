#pragma once

#include <string>
#include <map>
#include <vector>
#include <memory>

class SimulationJob
{
    public:
    SimulationJob() = default;
    ~SimulationJob() = default;

    explicit SimulationJob(const std::string& job_name);

    void AddRunBeforeJob(std::pair<std::string, std::shared_ptr<SimulationJob>> run_before);
    void AddRunAfterJob(std::pair<std::string, std::shared_ptr<SimulationJob>> run_after);
    void AddDependency(std::pair<std::string, std::shared_ptr<SimulationJob>> dependency);

    bool CanRun() const;

    bool IsComplete() const;

    void Execute(double deltaTime);

    inline std::string& GetName() { return mName; }

    inline void SetJobRate(double job_rate);
    inline void SetJobRateTics(size_t job_rate_tics);
    inline double GetJobRate() const;
    inline size_t GetJobRateTics() const;

    inline void SetNextRunTics(size_t next_run_tics);
    inline size_t GetNextRunTics() const;

    inline void SetLastRunTics(size_t last_run_tics);
    inline size_t GetLastRunTics() const;

    protected:
    virtual void RunJob(double deltaTime); 
    bool mComplete{false};
    std::string mName;
    
    double mJobRate{0.0};
    size_t mJobRateTics{0};
    size_t mNextRunTics{0};
    size_t mLastRanTics{0};
    
    private:

    std::map<std::string, std::vector<std::shared_ptr<SimulationJob>>> mDependsOnJobs; 
    std::map<std::string, std::vector<std::shared_ptr<SimulationJob>>> mRunBeforeJobs;
    std::map<std::string, std::vector<std::shared_ptr<SimulationJob>>> mRunAfterJobs;
    
   
};


inline void SimulationJob::SetJobRate(double job_rate)
{
    mJobRate = job_rate;
}
inline void SimulationJob::SetJobRateTics(size_t job_rate_tics)
{
    mJobRateTics = job_rate_tics;
}
inline double SimulationJob::GetJobRate() const
{
    return mJobRate;
}
inline size_t SimulationJob::GetJobRateTics() const
{
    return mJobRateTics;
}

inline void SimulationJob::SetNextRunTics(size_t next_run_tics)
{
    mNextRunTics = next_run_tics;
}

inline size_t SimulationJob::GetNextRunTics() const
{
    return mNextRunTics;
}

inline void SimulationJob::SetLastRunTics(size_t last_run_tics)
{
    mLastRanTics = last_run_tics;
}

inline size_t SimulationJob::GetLastRunTics() const
{
    return mLastRanTics;
}
