cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(simple3d VERSION 0.0.0 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/simple3d/cmake ${CMAKE_MODULE_PATH})

include(utils)
include(user-config.cmake OPTIONAL)

include(ExternalProject)

# Flag for determining how the library is built.
# If IS_MAIN_PROJECT is false, web client part should be downloaded instead of being built.
# TODO(apachee): Download release using link pattern from
# FIXME(apachee): Get info about release using github API in cmake
# https://docs.github.com/en/repositories/releasing-projects-on-github/linking-to-releases.
set(IS_MAIN_PROJECT OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(IS_MAIN_PROJECT ON)
	simple3d_init_dev_cmake_options()
endif()

set(SIMPLE3D_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SIMPLE3D_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(SIMPLE3D_WEB_CLIENT_SOURCE_DIR "${SIMPLE3D_SOURCE_DIR}/simple3d/web/client")
set(SIMPLE3D_WEB_CLIENT_BINARY_DIR "${SIMPLE3D_BINARY_DIR}/simple3d/web/client")

simple3d_report_variables(
	"simple3d"
	IS_MAIN_PROJECT
	CMAKE_BUILD_TYPE
	SIMPLE3D_SOURCE_DIR
	SIMPLE3D_BINARY_DIR)

add_subdirectory(simple3d)

# TODO(apachee): Make install steps that puts html files where they belong
# TODO(apachee): Maybe use distinct build directories for different build types and create compile_commands.json symlink in project root.
# This way CMake can change symlink according to build type selected (this way you can potentially look at clangd errors for differnet configurations). Another benefit is that you can build different configurations 
# without affecting one another, thus preserving caches.