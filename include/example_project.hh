#pragma once
#ifndef __EXAMPLE_PROJECT_HH__
#define __EXAMPLE_PROJECT_HH__

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>

class Logger {
    public:
    static std::shared_ptr<spdlog::logger> getInstance() {
        static std::shared_ptr<spdlog::logger> instance = spdlog::basic_logger_mt("basic_logger", "logs/log.log");
        return instance;
    }

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    private:
    Logger() {
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v"); // Set log pattern
    }
};


#endif