cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(apachee_simple3d_frontend VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${APACHEE_SIMPLE3D_SOURCE_DIR}/simple3d/cmake ${CMAKE_MODULE_PATH})

include(utils)

apachee_simple3d_init_dev_cmake_options()

execute_process(
	COMMAND em++ --cflags
	OUTPUT_VARIABLE EM_CFLAGS)

string(STRIP "${EM_CFLAGS}" EM_CFLAGS)

# file(GLOB_RECURSE APACHEE_SIMPLE3D_FRONTEND_SOURCES *.cpp)
set(APACHEE_SIMPLE3D_FRONTEND_SOURCES program.cpp graphics/webgl2/webgl.cpp)

set(APACHEE_SIMPLE3D_FRONTEND_EMSCRIPTEN_LINK_OPTIONS "SHELL:--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/templates/index.html" "SHELL:-sMIN_WEBGL_VERSION=2" "SHELL:-sMAX_WEBGL_VERSION=2")

set(APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE "dwarf" CACHE STRING "Either \"dwarf\" or \"source-map\"")
if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
	if (APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE STREQUAL "dwarf")
		add_compile_options("-g")
	elseif (APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE  STREQUAL "source-map")
		add_compile_options("-gsource-map")
	else()
		message(
			FATAL_ERROR
			"Unrecognized value of APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE: ${APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE}, "
			"this value should be either \"dwarf\" or \"source-map\"")
	endif()
endif()

apachee_simple3d_report_variables(
    "simple3d/simple3d/internal/frontend"
	CMAKE_BUILD_TYPE
	APACHEE_SIMPLE3D_SOURCE_DIR
	APACHEE_SIMPLE3D_BINARY_DIR
	APACHEE_SIMPLE3D_FRONTEND_SOURCES
	APACHEE_SIMPLE3D_FRONTEND_EMSCRIPTEN_LINK_OPTIONS
	APACHEE_SIMPLE3D_FRONTEND_DEBUG_INFO_TYPE)

add_subdirectory(${APACHEE_SIMPLE3D_SOURCE_DIR}/simple3d/core ${CMAKE_CURRENT_BINARY_DIR}/core)

add_executable(apachee_simple3d_frontend ${APACHEE_SIMPLE3D_FRONTEND_SOURCES})

target_link_libraries(apachee_simple3d_frontend PUBLIC apachee_simple3d_core)

target_include_directories(apachee_simple3d_frontend SYSTEM PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})

set_property(TARGET apachee_simple3d_frontend PROPERTY CXX_STANDARD 20)
set_property(TARGET apachee_simple3d_frontend PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")
set_property(TARGET apachee_simple3d_frontend PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")
set_property(TARGET apachee_simple3d_frontend PROPERTY OUTPUT_NAME index)
set_property(TARGET apachee_simple3d_frontend PROPERTY SUFFIX .html)

target_link_options(apachee_simple3d_frontend PUBLIC ${APACHEE_SIMPLE3D_FRONTEND_EMSCRIPTEN_LINK_OPTIONS})
