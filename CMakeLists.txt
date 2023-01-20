cmake_minimum_required(VERSION 3.21)

set(VCPKG_MANIFEST_MODE ON)
set(VCPKG_MANIFEST_INSTALL ON)
# set(VCPKG_MANIFEST_DIR ${CMAKE_CURRENT_SOURCE_DIR})

project(simple3d)

set(CMAKE_CXX_STANDARD 17)

include(${simple3d_SOURCE_DIR}/cmake/CPM.cmake)

# GLFW setup
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
CPMAddPackage("gh:glfw/glfw#3.3.8")

# GLM setup
CPMAddPackage("gh:g-truc/glm#0.9.9.8")

# assimp setup
CPMAddPackage("gh:assimp/assimp@5.2.5")

find_package(ZLIB REQUIRED)
find_package(unofficial-angle CONFIG REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(examples)

install(TARGETS
    examples::interactive_scene examples::interactive_model
    RUNTIME
        COMPONENT Development)
file(GLOB_RECURSE VCPKG_INSTALLED_BINARIES ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*)
install(FILES
    ${VCPKG_INSTALLED_BINARIES}
    TYPE BIN
    COMPONENT Development)

find_package(Doxygen)
if (DOXYGEN_FOUND)
    set(SIMPLE3D_BUILD_INTERNAL_DOCS ON CACHE BOOL "specify whether you need documentation of private API")
    set(SIMPLE3D_DOXYGEN_INPUT "${CMAKE_SOURCE_DIR}/include/simple3d/")

    if (${SIMPLE3D_BUILD_INTERNAL_DOCS})
        set(SIMPLE3D_DOXYGEN_INPUT ${SIMPLE3D_DOXYGEN_INPUT} "${CMAKE_SOURCE_DIR}/src/")
    endif()

    list(JOIN SIMPLE3D_DOXYGEN_INPUT " " SIMPLE3D_DOXYGEN_INPUT)

    configure_file(
        Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE}
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Target to generate docs" VERBATIM
    )
endif()