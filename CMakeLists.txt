cmake_minimum_required(VERSION 3.20 FATAL_ERROR)

project(simple3d VERSION 0.0.0 LANGUAGES C CXX)

set(CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/simple3d/cmake ${CMAKE_MODULE_PATH})

include(utils)
include(ci)
include(user-config.cmake OPTIONAL)

include(FetchContent)
include(ExternalProject)

# Flag for determining how the library is built.
# If IS_MAIN_PROJECT is false, web client part should be downloaded instead of being built.
set(IS_MAIN_PROJECT OFF)

if(CMAKE_CURRENT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR)
	set(IS_MAIN_PROJECT ON)
	simple3d_init_dev_cmake_options()
endif()

FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

if (IS_MAIN_PROJECT)
	enable_testing()

	FetchContent_Declare(
	googletest
	URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
	)

	set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

	FetchContent_MakeAvailable(googletest)

	include(GoogleTest)
endif()

set(SIMPLE3D_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(SIMPLE3D_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")

set(SIMPLE3D_WEB_CLIENT_SOURCE_DIR "${SIMPLE3D_SOURCE_DIR}/simple3d/web/client")
set(SIMPLE3D_WEB_CLIENT_BINARY_DIR "${SIMPLE3D_BINARY_DIR}/simple3d/web/client")

set(SIMPLE3D_WEB_CLIENT_GLM_SOURCE_DIR "${glm_SOURCE_DIR}")
set(SIMPLE3D_WEB_CLIENT_GLM_BINARY_DIR "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/_deps/glm/glm-build")

set(SIMPLE3D_FORCE_DOWNLOAD_WEB_CLIENT OFF CACHE BOOL "")

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