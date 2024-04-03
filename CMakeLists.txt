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
IF (WIN32)
	include_directories(libs/SDL2/include)
	include_directories(libs/SDL2_image)
	link_directories(libs/SDL2/lib/x64)
ENDIF()

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

IF (WIN32)
	target_link_libraries(${PROJECT_NAME} SDL2main)
	target_link_libraries(${PROJECT_NAME} SDL2)
	target_link_libraries(${PROJECT_NAME} SDL2_image)
ELSE()
	find_package(SDL2 REQUIRED)
	target_include_directories(${PROJECT_NAME} PRIVATE ${SDL2_INCLUDE_DIRS})


	if(CMAKE_SYSTEM_NAME STREQUAL "Linux")
		find_package(PkgConfig REQUIRED)

		pkg_search_module(SDL2IMAGE REQUIRED SDL2_image)
		pkg_search_module(SDL2NET REQUIRED SDL2_net)

		include_directories(${SDL2IMAGE_INCLUDE_DIRS})
		include_directories(${SDL2NET_INCLUDE_DIRS})

		target_link_libraries(${PROJECT_NAME} ${SDL2IMAGE_LIBRARIES})
		target_link_libraries(${PROJECT_NAME} ${SDL2NET_LIBRARIES})

		# Optionally, add the linker flags to your target
		target_link_options(${PROJECT_NAME} PRIVATE ${SDL2IMAGE_LDFLAGS})
		target_link_options(${PROJECT_NAME} PRIVATE ${SDL2NET_LDFLAGS})

	else ()
		find_package(SDL2_image REQUIRED)
		target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_image::SDL2_image)
		find_package(SDL2_net REQUIRED)
		target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_net::SDL2_net)
	endif ()

	find_package(Boost 1.71.0 REQUIRED)
	target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
	target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})
ENDIF()

