if(${PLATFORM_IS_PS2})
	add_subdirectory(PS2)
else()
	add_subdirectory(SDL2)
endif()

add_public_sources(platform renderer.h platform.h)
