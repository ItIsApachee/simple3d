cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(simple3d_web_client VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${SIMPLE3D_SOURCE_DIR}/simple3d/cmake ${CMAKE_MODULE_PATH})

include(utils)

simple3d_init_dev_cmake_options()

add_compile_definitions(ALLOW_BLOCKING_ON_MAIN_THREAD=0)

execute_process(
	COMMAND em++ --cflags
	OUTPUT_VARIABLE EM_CFLAGS)

string(STRIP "${EM_CFLAGS}" EM_CFLAGS)

# file(GLOB_RECURSE SIMPLE3D_WEB_CLIENT_SOURCES *.cpp)
set(
	SIMPLE3D_WEB_CLIENT_SOURCES
	program.cpp)

set(SIMPLE3D_WEB_CLIENT_EMSCRIPTEN_LINK_OPTIONS "SHELL:--shell-file ${CMAKE_CURRENT_SOURCE_DIR}/templates/index.html" "SHELL:-sMIN_WEBGL_VERSION=2" "SHELL:-sMAX_WEBGL_VERSION=2")

set(SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE "dwarf" CACHE STRING "Either \"dwarf\" or \"source-map\"")
if (CMAKE_BUILD_TYPE STREQUAL Debug OR CMAKE_BUILD_TYPE STREQUAL RelWithDebInfo)
	if (SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE STREQUAL "dwarf")
		add_compile_options("-g")
	elseif (SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE  STREQUAL "source-map")
		add_compile_options("-gsource-map")
	else()
		message(
			FATAL_ERROR
			"Unrecognized value of SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE: ${SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE}, "
			"this value should be either \"dwarf\" or \"source-map\"")
	endif()
endif()

# Enable threads
add_compile_options(-pthread)
add_link_options(
	-pthread
	"SHELL:-sPTHREAD_POOL_SIZE=\"Math.max(2, navigator.hardwareConcurrency)\""
	"SHELL:-sPROXY_TO_PTHREAD")

# Enable offscreen canvas

add_link_options(
	"SHELL:-sOFFSCREENCANVAS_SUPPORT"
	"SHELL:-sOFFSCREEN_FRAMEBUFFER"
	"SHELL:-sOFFSCREENCANVASES_TO_PTHREAD=#canvas")

simple3d_report_variables(
    "simple3d/simple3d/web/client"
	CMAKE_BUILD_TYPE
	SIMPLE3D_SOURCE_DIR
	SIMPLE3D_BINARY_DIR
	SIMPLE3D_WEB_CLIENT_SOURCES
	SIMPLE3D_WEB_CLIENT_EMSCRIPTEN_LINK_OPTIONS
	SIMPLE3D_WEB_CLIENT_DEBUG_INFO_TYPE)

add_subdirectory(${SIMPLE3D_SOURCE_DIR}/simple3d/core ${CMAKE_CURRENT_BINARY_DIR}/core)
add_subdirectory(lib)
add_subdirectory(gl)

add_executable(simple3d_web_client ${SIMPLE3D_WEB_CLIENT_SOURCES})

target_link_libraries(simple3d_web_client PUBLIC simple3d_core simple3d_web_client_lib simple3d_web_client_gl)

target_include_directories(simple3d_web_client SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

set_property(TARGET simple3d_web_client PROPERTY CXX_STANDARD 20)
set_property(TARGET simple3d_web_client PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")
set_property(TARGET simple3d_web_client PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")
set_property(TARGET simple3d_web_client PROPERTY OUTPUT_NAME index)
set_property(TARGET simple3d_web_client PROPERTY SUFFIX .html)

target_link_options(simple3d_web_client PUBLIC ${SIMPLE3D_WEB_CLIENT_EMSCRIPTEN_LINK_OPTIONS})
