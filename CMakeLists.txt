cmake_minimum_required(VERSION 3.25)

set(PROJECT_NAME 2d_multiplayer_shooter)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)

add_executable(2d_multiplayer_shooter main.cpp)

target_compile_options(${PROJECT_NAME} PUBLIC -O3)

# Adding boost
find_package(Boost 1.71.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
