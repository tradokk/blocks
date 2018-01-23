if(CMAKE_SOURCE_DIR EQUAL CMAKE_BINARY_DIR)
	message(FATAL_ERROR "Builds within the source directory are not allowed!")
endif()

cmake_minimum_required (VERSION 3.7)

set (PROJECT_NAME tetris)
project (${PROJECT_NAME} C)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type, 
		options are: None, Debug, Release, RelWithDebInfo, MinSizeRel." FORCE)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

include(AddSources)

set(CMAKE_C_STANDARD 99)

if(CMAKE_COMPILER_IS_GNUC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

if(${PLATFORM_IS_PS2})
	set(CMAKE_TOOLCHAIN_FILE "${CMAKE_MODULE_PATH}/PS2Toolchain.cmake")
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=c99")
	include_directories(${CMAKE_SOURCE_DIR}/src/platform/PS2)
	set(LIBRARIES "-lpacket -ldma -lgraph -ldraw -lmf")
else()
	find_package(SDL2 REQUIRED)
	include_directories(${SDL2_INCLUDE_DIR})
	set(LIBRARIES ${SDL2_LIBRARY})
endif()

find_package(PythonInterp)

include_directories(${CMAKE_SOURCE_DIR}/src)

add_subdirectory(${CMAKE_SOURCE_DIR}/data)
add_subdirectory(src)

install(DIRECTORY data DESTINATION bin)
