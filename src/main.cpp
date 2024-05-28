#include <iostream>
#include <string>
#include <csignal>
#include <chrono>


#include "example_project.hh"


std::atomic<bool> running{true};


void signalHandler(int signal) {
    if(signal == SIGINT) {
        running = false;
        auto console_logger = ConsoleLogger::getInstance();
        console_logger->warn("Got a SIGINT, shutting down");
    }
}

int main() 
{
    std::signal(SIGINT, signalHandler);

    std::cout << "Hello world!  Here's a simple CMake Example with two dependencies.  One for testing, one for runtime." << std::endl;

    auto file_logger = FileLogger::getInstance();
    auto console_logger = ConsoleLogger::getInstance();

    console_logger->info("Starting the loop!");

    while(running) {

        console_logger->info("Running...");
        std::this_thread::sleep_for(std::chrono::seconds(1));
    } 



    return EXIT_SUCCESS;
}


