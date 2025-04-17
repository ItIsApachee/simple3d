cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(apachee_simple3d_frontend VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${APACHEE_SIMPLE3D_SOURCE_DIR}/simple3d/cmake ${CMAKE_MODULE_PATH})

include(utils)

report_variables(
    "simple3d/simple3d/internal/frontend"
		APACHEE_SIMPLE3D_SOURCE_DIR
		APACHEE_SIMPLE3D_BINARY_DIR)

execute_process(
	COMMAND em++ --cflags
	OUTPUT_VARIABLE EM_CFLAGS)

string(STRIP "${EM_CFLAGS}" EM_CFLAGS)

add_executable(apachee_simple3d_frontend test.cpp)

target_include_directories(apachee_simple3d_frontend PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})

set_property(TARGET apachee_simple3d_frontend PROPERTY CXX_STANDARD 20)
set_property(TARGET apachee_simple3d_frontend PROPERTY RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/out")
set_property(TARGET apachee_simple3d_frontend PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")
set_property(TARGET apachee_simple3d_frontend PROPERTY OUTPUT_NAME index)
set_property(TARGET apachee_simple3d_frontend PROPERTY SUFFIX .js)

add_custom_command(
	TARGET apachee_simple3d_frontend
	POST_BUILD
	COMMAND ${CMAKE_COMMAND} -E copy_if_different "${CMAKE_CURRENT_SOURCE_DIR}/templates/index.html" "${CMAKE_CURRENT_BINARY_DIR}/out"
	BYPRODUCTS "${CMAKE_CURRENT_BINARY_DIR}/out/index.html")
