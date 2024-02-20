cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME 2d_multiplayer_shooter)

project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(PROFILING TRUE)
set(USING_VISUAL_STUDIO TRUE)
set(ENABLE_FAST_MATH ON)

include(FetchContent)
include_directories(include)
include_directories(libs)
include_directories(libs/SDL2/include)
include_directories(include)

link_directories(libs/SDL2/lib/x64)

add_executable(${PROJECT_NAME} 
	"src/main.cpp"
	"src/Input.cpp"
	"src/Object.cpp"
	"src/SDLHandler.cpp"
)

find_package(Boost 1.71.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2main)

target_compile_options(${PROJECT_NAME} PUBLIC -O3)
