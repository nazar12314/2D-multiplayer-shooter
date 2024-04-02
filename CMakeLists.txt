cmake_minimum_required(VERSION 3.16)

set(PROJECT_NAME 2d_multiplayer_shooter)
project(${PROJECT_NAME} CXX)

set(CMAKE_CXX_STANDARD 20)
set(PROFILING TRUE)
set(USING_VISUAL_STUDIO TRUE)
set(ENABLE_FAST_MATH ON)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

include_directories(include)
include_directories(include/Component)
include_directories(include/Component/Custom)
include_directories(include/Component/Collider)
include_directories(include/Core)
include_directories(include/Core/Physics)
include_directories(include/Object)
include_directories(include/Other)
include_directories(include/Utils)

include_directories(libs)
include_directories(libs/SDL2/include)
include_directories(libs/SDL2_image)
link_directories(libs/SDL2/lib/x64)

add_executable(${PROJECT_NAME} 
	 "src/main.cpp"
	 "src/Core/Input.cpp"
	 "src/Object/Object.cpp"
	 "src/Application.cpp"
	 "src/Object/Transform.cpp"
	 "src/Core/Renderer.cpp"
	 "src/Component/Camera.cpp"
	 "src/Core/SDLHandler.cpp"
	 "src/Component/SpriteRenderer.cpp"
	 "src/Component/Component.cpp"
	 "src/Component/Custom/CameraFollow.cpp"
	 "src/Scene.cpp"
	 "src/Component/Custom/TankController.cpp"
	 "src/Component/Collider/PolygonCollider.cpp"
	 "src/Component/Collider/CircleCollider.cpp"
	 "src/Component/Rigidbody.cpp"
	 "src/Core/Physics/Physics.cpp"
	 "src/Core/MyTime.cpp"
	 "src/Component/Custom/Tank.cpp"
	 "src/Utils/MyMath.cpp" 
	 "src/Component/Custom/Bullet.cpp"
	 "src/Component/Custom/CameraResizer.cpp"
	 "include/Core/FPSCounter.h"
	 "src/Component/Collider/Collider.cpp"
	 "src/Component/Custom/Wall.cpp"
	 "src/Multiplayer/Client.cpp"
	 "src/Multiplayer/Server.cpp"
	 "src/Multiplayer/MessageQueue.cpp"
	 "include/Multiplayer/Client.h"
	 "include/Multiplayer/Server.h"
	 "include/Multiplayer/MessageQueue.h"
	 "src/Core/Assets.cpp"
	 "include/Utils/Concepts.h"
	 "src/Utils/Color.cpp"
	 "src/Other/Material.cpp"
	 "src/Core/Physics/Solver.cpp"
	 "include/Core/Physics/Collision.h"
	 "src/Other/Gizmos.cpp"
	 "include/Utils/SDLUtils.h" "include/Component/Custom/ShapeSpawner.h" "src/Component/Custom/ShapeSpawner.cpp")

add_subdirectory(libs/SDL2_image)

target_link_libraries(${PROJECT_NAME} SDL2main)
target_link_libraries(${PROJECT_NAME} SDL2)
target_link_libraries(${PROJECT_NAME} SDL2_image)
