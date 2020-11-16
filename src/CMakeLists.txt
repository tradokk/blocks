add_executable(tetris main.c)
target_sources(tetris PUBLIC 
                              "${CMAKE_CURRENT_SOURCE_DIR}/game.h" 
                              "${CMAKE_CURRENT_SOURCE_DIR}/draw_game.h"
					  		  "${CMAKE_CURRENT_SOURCE_DIR}/resource.h"
					  		  "${CMAKE_CURRENT_SOURCE_DIR}/renderer.h"
					  		  "${CMAKE_CURRENT_SOURCE_DIR}/generic_renderer.h"
					  		  "${CMAKE_CURRENT_SOURCE_DIR}/platform.h"
					  PRIVATE
							  "${CMAKE_CURRENT_SOURCE_DIR}/game.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/draw_game.c"
                              "${CMAKE_CURRENT_SOURCE_DIR}/generic_renderer.c")


target_include_directories(tetris PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")

target_link_libraries(tetris PRIVATE resource)


add_library(tetris_headers INTERFACE)
target_include_directories(tetris_headers INTERFACE "${CMAKE_CURRENT_SOURCE_DIR}")

if(EMSCRIPTEN)
	target_compile_options(tetris PUBLIC "-s MODULARIZE=1" "-s EXPORT_ES6=1" "-s USE_ES6_IMPORT_META=0")
	target_compile_definitions(tetris PUBLIC "UI")
	#set(CMAKE_EXECUTABLE_SUFFIX ".html")
	target_sources(tetris PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/emscripten_platform.c")
else()
	target_compile_definitions(tetris PUBLIC "COLOR_BGRA")
	find_package(SDL2 REQUIRED)
	target_sources(tetris PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/platforms/sdl2_platform.c")
	target_link_libraries(tetris PRIVATE SDL2::SDL2)
endif()

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
  set_property(TARGET tetris PROPERTY INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
endif()


install(TARGETS tetris RUNTIME DESTINATION bin)
