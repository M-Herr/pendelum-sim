cmake_minimum_required(VERSION 3.0)

project(
    ExampleProject
    VERSION 1.0
    LANGUAGES CXX)


find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.0.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(external/rl-tools)

set(PROJECT_INCLUDE_DIR
${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_SOURCE_DIR 
${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PROJECT_HEADER
${PROJECT_INCLUDE_DIR}/example_project.hh)

set(PROJECT_SOURCE 
${PROJECT_SOURCE_DIR}/main.cpp
${PROJECT_SOURCE_DIR}/PIDController.cpp
${PROJECT_SOURCE_DIR}/InvertedPendelum.cpp
${PROJECT_SOURCE_DIR}/NeuralNetwork.cpp
${PROJECT_SOURCE_DIR}/RLAgent.cpp
${PROJECT_SOURCE_DIR}/Simulation.cpp
${PROJECT_SOURCE_DIR}/SimulationJob.cpp
${PROJECT_SOURCE_DIR}/PIDTuningJob.cpp
${PROJECT_SOURCE_DIR}/IntegratorBase.cpp
${PROJECT_SOURCE_DIR}/RungeKutta4.cpp
${PROJECT_SOURCE_DIR}/IntegratorJob.cpp
${PROJECT_SOURCE_DIR}/VariableRegistry.cpp
)

add_executable(ExampleProject ${PROJECT_SOURCE} ${PROJECT_HEADER})

target_include_directories(ExampleProject 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    
    )

target_link_libraries(ExampleProject 
    PRIVATE GTest::gtest_main spdlog::spdlog SFML::Graphics SFML::Window SFML::System RLtools::RLtools
    )

#set_target_properties(ExampleProject PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/example_project.hh)

install(TARGETS ExampleProject DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib")