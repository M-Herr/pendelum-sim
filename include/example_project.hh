#pragma once
#ifndef __EXAMPLE_PROJECT_HH__
#define __EXAMPLE_PROJECT_HH__

#include <memory>

#include <spdlog/spdlog.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <vector>


class ConsoleLogger 
{
    public:
    static std::shared_ptr<spdlog::logger> getInstance() {
            static std::shared_ptr<spdlog::logger> instance = spdlog::stdout_color_mt("console_logger");
            return instance;
        }

    ConsoleLogger(const ConsoleLogger&) = delete;
    ConsoleLogger& operator=(const ConsoleLogger&) = delete;
    private:
    ConsoleLogger() {
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v"); // Set log pattern
    }
};

class FileLogger {
    public:

    static std::shared_ptr<spdlog::logger> getInstance() {
        static std::shared_ptr<spdlog::logger> instance = spdlog::basic_logger_mt("file_logger", "logs/log.log");
        return instance;
    }
    
    FileLogger(const FileLogger&) = delete;
    FileLogger& operator=(const FileLogger&) = delete;

    private:
    FileLogger() {
        spdlog::set_level(spdlog::level::debug);
        spdlog::set_pattern("[%Y-%m-%d %H:%M:%S.%e] [%l] [%n] %v"); // Set log pattern
    }
};


#endif