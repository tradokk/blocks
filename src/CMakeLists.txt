#add_library(resource resource.h generated/interface.cpp)

add_subdirectory(platform)
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/platform)

add_executable(${PROJECT_NAME} main.c)
add_sources(${PROJECT_NAME} game.c game.h draw_game.c draw_game.h resource.h)
target_link_libraries(${PROJECT_NAME}  platform resource ${LIBRARIES})

install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
