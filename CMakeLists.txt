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
include_directories(include/Component/Collider)
include_directories(include/Component/Renderer)
include_directories(include/Core)
include_directories(include/Custom)
include_directories(include/Custom/Example)
include_directories(include/Core/Physics)
include_directories(include/Object)
include_directories(include/Other)
include_directories(include/Utils)

include_directories(libs)
IF (WIN32)
	include_directories(libs/SDL2/include)
	include_directories(libs/SDL2_image)
	include_directories(libs/SDL2_ttf)
	link_directories(libs/SDL2/lib/x64)
ENDIF()

add_executable(${PROJECT_NAME} 
	 "src/main.cpp"
	 "src/Core/Input.cpp"
	 "src/Object/GameObject.cpp"
	 "src/Application.cpp"
	 "src/Component/Transform.cpp"
	 "src/Core/Renderer.cpp"
	 "src/Component/Camera.cpp"
	 "src/Core/SDLHandler.cpp"
	 "src/Component/Renderer/SpriteRenderer.cpp"
	 "src/Component/Component.cpp"
	 "src/Custom/Example/ECameraFollow.cpp"
	 "src/Scene.cpp"
	 "src/Custom/Example/ETankController.cpp"
	 "src/Component/Collider/PolygonCollider.cpp"
	 "src/Component/Collider/CircleCollider.cpp"
	 "src/Component/Rigidbody.cpp"
	 "src/Core/Physics/Physics.cpp"
	 "src/Core/MyTime.cpp"
	 "src/Custom/Example/ETank.cpp"
	 "src/Utils/MyMath.cpp" 
	 "src/Custom/Example/EBullet.cpp"
	 "src/Custom/Example/ECameraResizer.cpp"
	 "include/Core/FPSCounter.h"
	 "src/Component/Collider/Collider.cpp"
	 "src/Custom/Example/EWall.cpp"
	 "src/Core/Assets.cpp"
	 "include/Utils/Concepts.h"
	 "src/Utils/Color.cpp"
	 "src/Other/Texture.cpp"
	 "src/Core/Physics/Solver.cpp"
	 "include/Core/Physics/Collision.h"
	 "src/Other/Gizmos.cpp"
	 "include/Utils/SDLUtils.h"
	 "src/Custom/Example/EShapeSpawner.cpp"
	 "include/Utils/Utils.h"
	 "src/Component/Renderer/TextRenderer.cpp"
	 "src/Component/Renderer/BaseRenderer.cpp"
	  "src/Other/Tweener.cpp"
	  "src/Object/Object.cpp"
	  "src/Utils/Utils.cpp"
	  "src/Custom/Example/ExampleScenes.cpp"
	  "src/Custom/CameraFollow.cpp"
	  "src/Custom/CameraResizer.cpp"
	  "src/Custom/Tank.cpp"
	  "src/Custom/TankPlayerController.cpp"
	  "src/Custom/Bullet.cpp"
	  "include/Custom/Singleton.h"
	  "src/Custom/MapManager.cpp"
	  "include/Multiplayer/Client.h"
	  "include/Multiplayer/Server.h"
	  "src/Component/Renderer/ParticleSystem.cpp"
	  "src/Custom/TankRemoteController.cpp"
	  "src/Custom/PlayerManager.cpp"
	  "src/Custom/Multiplayer.cpp"
	  "src/Multiplayer/net_modules.cpp"
      "include/Utils/Debug.h")

IF (WIN32)
	target_link_libraries(${PROJECT_NAME} SDL2main)
	target_link_libraries(${PROJECT_NAME} SDL2)
	target_link_libraries(${PROJECT_NAME} SDL2_image)
	target_link_libraries(${PROJECT_NAME} SDL2_ttf)

	configure_file(libs/SDL2_image/SDL2_image.dll ${CMAKE_BINARY_DIR}/SDL2_image.dll COPYONLY)
	configure_file(libs/SDL2_image/SDL2_image.lib ${CMAKE_BINARY_DIR}/SDL2_image.lib COPYONLY)
	configure_file(libs/SDL2_ttf/SDL2_ttf.dll ${CMAKE_BINARY_DIR}/SDL2_ttf.dll COPYONLY)
	configure_file(libs/SDL2_ttf/SDL2_ttf.lib ${CMAKE_BINARY_DIR}/SDL2_ttf.lib COPYONLY)

	set(_WIN32_WINDOWS)
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
		find_package(SDL2_ttf REQUIRED)
		target_link_libraries(${PROJECT_NAME} PRIVATE ${SDL2_LIBRARIES} SDL2_ttf::SDL2_ttf)
	endif ()
ENDIF()

find_package(Boost 1.71.0 REQUIRED)
target_include_directories(${PROJECT_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE ${Boost_LIBRARIES})

