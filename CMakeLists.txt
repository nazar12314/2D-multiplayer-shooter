cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME 2d_multiplayer_shooter)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(PROFILING TRUE)
set(USING_VISUAL_STUDIO TRUE)
set(ENABLE_FAST_MATH ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${ADDITIONAL_CFLAGS} -g")

include_directories(include)
include_directories(include/Component)
include_directories(include/Component/Custom)
include_directories(include/Component/Collider)
include_directories(include/Core)
include_directories(include/Object)
include_directories(include/Other)
include_directories(include/Utils)

include_directories(libs)

add_executable(${PROJECT_NAME} 
	 "src/main.cpp"
	 "src/Core/Input.cpp"
	 "src/Object/Object.cpp"
	 "src/Application.cpp"
	 "src/Object/Transform.cpp"
	 "src/Core/Renderer.cpp"
	 "include/Other/Texture.h"
	 "src/Component/Camera.cpp"
	 "src/Core/SDLHandler.cpp"
	 "src/Component/Sprite.cpp"
	 "src/Component/Component.cpp"
	 "src/Component/Custom/CameraFollow.cpp"
	 "src/Other/Texture.cpp"
	 "src/Scene.cpp"
	 "src/Component/Custom/TankController.cpp"
	 "src/Component/Collider/PolygonCollider.cpp" 
	 "src/Component/Collider/CircleCollider.cpp"
	 "src/Component/Rigidbody.cpp"
	 "src/Core/Physics.cpp"
	 "src/Core/MyTime.cpp"
	 "src/Component/Custom/Tank.cpp" "include/Utils/Math.h" "src/Utils/Math.cpp" "include/Component/Custom/Bullet.h" "src/Component/Custom/Bullet.cpp")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)

message("${SDL2_INCLUDE_DIRS}")

find_package(Boost 1.71.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
