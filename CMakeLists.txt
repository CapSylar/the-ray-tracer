cmake_minimum_required(VERSION 3.16)
project(ray_tracer C)

set(CMAKE_C_STANDARD 11)

SET(GCC_COVERAGE_COMPILE_FLAGS "")
SET(GCC_COVERAGE_LINK_FLAGS    "-lcheck -lm")

SET(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} ${GCC_COVERAGE_LINK_FLAGS}")


set( COMPILE_TESTS FALSE )

if ( COMPILE_TESTS )
    set(ADD_SOURCES tests.c)
else()
    set( ADD_SOURCES main.c )

endif()

add_executable( ray_tracer defs.h factories.c factories.h ${ADD_SOURCES} canvas.c canvas.h matrix_routines.c matrix_routines.h trans_routines.c trans_routines.h ray.c ray.h world_view.c world_view.h)