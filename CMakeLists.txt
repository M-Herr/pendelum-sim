cmake_minimum_required(VERSION 3.0)

project(
    ExampleProject
    VERSION 1.0
    LANGUAGES CXX)


find_package(gtest REQUIRED)


set(CMAKE_CXX_STANDARD 20)

set(PROJECT_SOURCE 
${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)


add_executable(ExampleProject ${PROJECT_SOURCE})