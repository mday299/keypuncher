cmake_minimum_required(VERSION 3.10) 

# Project name and version 
project(TelemetryProcessor VERSION 1.0) 

# Specify C++ standard 
set(CMAKE_CXX_STANDARD 17) 
set(CMAKE_CXX_STANDARD_REQUIRED True) 

# Add the executable for your main application 
add_executable(telemetry_processor main.cpp telemetry_processor.cpp) 

# Include directories target_
include_directories(telemetry_processor PRIVATE ${PROJECT_SOURCE_DIR}) 

# Google Test setup for unit testing 
enable_testing() 

# Add the executable for your tests 
add_executable(test_application test_telemetry_processor.cpp telemetry_processor.cpp) 

# Link Google Test libraries 
find_package(GTest REQUIRED) 
target_link_libraries(test_application GTest::GTest GTest::Main) 

# Add testing targets 
add_test(NAME TelemetryTests COMMAND test_application)