if(PLATFORM STREQUAL "SDL2")
	add_subdirectory(SDL2)
endif()
add_public_sources(platform renderer.h platform.h)
