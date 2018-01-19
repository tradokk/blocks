add_library(platform ps2_platform.c)
add_sources(platform ../generic_renderer.h ../generic_renderer.c)
add_public_sources(platform stdint.h)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})
