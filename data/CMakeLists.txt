set(TOOLS_DIR "${CMAKE_SOURCE_DIR}/tools")

add_custom_command(OUTPUT interface.c
	COMMAND ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/createtiles.py" interface
	"${CMAKE_SOURCE_DIR}/data/interface.bmp"
	"${CMAKE_CURRENT_BINARY_DIR}/interface.c"
	DEPENDS interface.bmp)

add_custom_command(OUTPUT blocks.c
	COMMAND ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/createtiles.py" blocks
	"${CMAKE_SOURCE_DIR}/data/blocks.bmp"
	"${CMAKE_CURRENT_BINARY_DIR}/blocks.c"
	DEPENDS blocks.bmp)

add_custom_command(OUTPUT font.c
	COMMAND ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/createfont.py" font 
	"${CMAKE_SOURCE_DIR}/data/font.bmp"
	"${CMAKE_CURRENT_BINARY_DIR}/font.c"
	DEPENDS font.bmp)

add_custom_command(OUTPUT cross.c
	COMMAND ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/createtiles.py" cross
	"${CMAKE_SOURCE_DIR}/data/cross.bmp"
	"${CMAKE_CURRENT_BINARY_DIR}/cross.c"
	DEPENDS cross.bmp)

add_custom_command(OUTPUT button.c
	COMMAND ${PYTHON_EXECUTABLE} "${TOOLS_DIR}/createtiles.py" button
	"${CMAKE_SOURCE_DIR}/data/button.bmp"
	"${CMAKE_CURRENT_BINARY_DIR}/button.c"
	DEPENDS button.bmp)


add_library(resource STATIC) 
target_sources(resource PRIVATE "${CMAKE_CURRENT_BINARY_DIR}/interface.c"
	            "${CMAKE_CURRENT_BINARY_DIR}/blocks.c"
				"${CMAKE_CURRENT_BINARY_DIR}/font.c"
				"${CMAKE_CURRENT_BINARY_DIR}/cross.c"
				"${CMAKE_CURRENT_BINARY_DIR}/button.c")
target_link_libraries(resource PRIVATE tetris_headers)
