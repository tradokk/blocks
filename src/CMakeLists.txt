#add_library(resource resource.h generated/interface.cpp)
add_executable(${PROJECT_NAME} main.c)
add_sources(${PROJECT_NAME} game.c game.h draw.c draw.h generic_renderer.h generic_renderer.c resource.h)
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY} resource)

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
