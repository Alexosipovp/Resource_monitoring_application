cmake_minimum_required(VERSION 3.10)
project(Resource_monitoring_application VERSION 0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Resource_monitoring_application src/main.cpp src/sources/test.cpp)
target_include_directories(Resource_monitoring_application PRIVATE ${./src/headers})