cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME 2d_multiplayer_shooter)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(PROFILING TRUE)
set(USING_VISUAL_STUDIO TRUE)
set(ENABLE_FAST_MATH ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")

include_directories(include)
include_directories(libs)

add_executable(${PROJECT_NAME} 
	 "src/main.cpp"
	 "src/Input.cpp"
	 "src/Object.cpp"
	 "src/Application.cpp"
	 "src/Transform.cpp"
	 "include/MyTime.h"
	 "src/Renderer.cpp"
	 "include/Texture.h"
	 "src/Camera.cpp"
	 "src/SDLHandler.cpp"
	 "src/Multiplayer.cpp")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_net REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)

target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_net::SDL2_net)

find_package(Boost 1.71.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
