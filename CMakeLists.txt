cmake_minimum_required (VERSION 3.11)

project (tetris C)

if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the build type" FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
		         "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
endif()
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/Modules/")

set(CMAKE_C_STANDARD 11)

if(CMAKE_COMPILER_IS_GNUC)
	set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall")
endif()

find_package(PythonInterp)

add_subdirectory(src)
add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/data)

install(DIRECTORY data DESTINATION bin)
