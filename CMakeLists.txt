cmake_minimum_required(VERSION 3.0)

project(
    ExampleProject
    VERSION 1.0
    LANGUAGES CXX)


find_package(GTest REQUIRED)
find_package(spdlog REQUIRED)


set(CMAKE_CXX_STANDARD 20)

set(PROJECT_INCLUDE_DIR
${CMAKE_CURRENT_SOURCE_DIR}/include)

set(PROJECT_SOURCE_DIR 
${CMAKE_CURRENT_SOURCE_DIR}/src)

set(PROJECT_HEADER
${PROJECT_INCLUDE_DIR}/example_project.hh)

set(PROJECT_SOURCE 
${PROJECT_SOURCE_DIR}/main.cpp)

add_executable(ExampleProject ${PROJECT_SOURCE} ${PROJECT_HEADER})

target_include_directories(ExampleProject 
    PUBLIC 
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    
    )

target_link_libraries(ExampleProject PRIVATE GTest::gtest_main spdlog::spdlog)

#set_target_properties(ExampleProject PROPERTIES PUBLIC_HEADER ${CMAKE_CURRENT_SOURCE_DIR}/include/example_project.hh)

install(TARGETS ExampleProject DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        RUNTIME DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/bin"
        LIBRARY DESTINATION "${CMAKE_CURRENT_SOURCE_DIR}/lib")