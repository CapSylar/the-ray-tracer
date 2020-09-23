cmake_minimum_required(VERSION 3.16)
project(ray_tracer C)

set(CMAKE_C_STANDARD 11)

find_package(PkgConfig REQUIRED)
pkg_check_modules(SDL2 REQUIRED sdl2)
pkg_check_modules(SDL2IMAGE REQUIRED SDL2_image)

SET(GCC_COVERAGE_COMPILE_FLAGS "-Wall")
SET(GCC_COVERAGE_LINK_FLAGS    "-lcheck -O0 ")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")


set( COMPILE_TESTS FALSE )

if ( COMPILE_TESTS )
    set(ADD_SOURCES tests.c)
else()
    set( ADD_SOURCES main.c )

endif()

add_executable( ray_tracer defs.h factories.c factories.h ${ADD_SOURCES} canvas.c canvas.h matrix_routines.c matrix_routines.h trans_routines.c trans_routines.h ray.c ray.h world_view.c world_view.h patterns.c patterns.h linked_list.c linked_list.h interface.c interface.h)


INCLUDE_DIRECTORIES(${SDL2_INCLUDE_DIRS} ${SDL2IMAGE_INCLUDE_DIRS})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} ${SDL2_LIBRARIES} ${SDL2IMAGE_LIBRARIES})
TARGET_LINK_LIBRARIES(${PROJECT_NAME} m ) # link math library