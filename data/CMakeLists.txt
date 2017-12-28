set(TOOLS_DIR ${CMAKE_SOURCE_DIR}/tools)

add_custom_command(OUTPUT interface.c
	COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/createtiles.py interface
	${CMAKE_SOURCE_DIR}/data/interface.bmp
	${CMAKE_CURRENT_BINARY_DIR}/interface.c
	DEPENDS interface.bmp)

add_custom_command(OUTPUT blocks.c
	COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/createtiles.py blocks
	${CMAKE_SOURCE_DIR}/data/blocks.bmp
	${CMAKE_CURRENT_BINARY_DIR}/blocks.c
	DEPENDS blocks.bmp)

add_custom_command(OUTPUT text.c
	COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/createtiles.py text
	${CMAKE_SOURCE_DIR}/data/text.bmp
	${CMAKE_CURRENT_BINARY_DIR}/text.c
	DEPENDS text.bmp)

add_custom_command(OUTPUT numbers.c
	COMMAND ${PYTHON_EXECUTABLE} ${TOOLS_DIR}/createfont.py numbers
	${CMAKE_SOURCE_DIR}/data/numbers.bmp
	${CMAKE_CURRENT_BINARY_DIR}/numbers.c
	DEPENDS numbers.bmp)


add_library(resource ${CMAKE_SOURCE_DIR}/src/resource.h)
target_sources(resource PRIVATE ${CMAKE_CURRENT_BINARY_DIR}/interface.c
	            ${CMAKE_CURRENT_BINARY_DIR}/blocks.c
				${CMAKE_CURRENT_BINARY_DIR}/text.c
				${CMAKE_CURRENT_BINARY_DIR}/numbers.c)
