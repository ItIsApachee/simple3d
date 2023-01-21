cmake_minimum_required(VERSION 3.21)

set(VCPKG_MANIFEST_MODE ON)
set(VCPKG_MANIFEST_INSTALL ON)

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
set(ASSIMP_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(ASSIMP_INSTALL OFF CACHE BOOL "" FORCE)
set(ASSIMP_BUILD_ASSIMP_VIEW OFF CACHE BOOL "" FORCE)
CPMAddPackage("gh:assimp/assimp@5.2.5")

find_package(ZLIB REQUIRED)
find_package(unofficial-angle CONFIG REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(examples)

# TODO: add targets for ANGLE, and try using install(EXPORT)
install(TARGETS
            simple3d-interactive-scene simple3d-interactive-model
        RUNTIME
            DESTINATION bin/
            CONFIGURATIONS Debug)
install(TARGETS
            simple3d-interactive-scene simple3d-interactive-model
        RUNTIME
            DESTINATION bin/
            CONFIGURATIONS Release RelWithDebInfo MinSizeRel)

file(GLOB_RECURSE VCPKG_INSTALLED_BINARIES_DEBUG ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/*)
file(GLOB_RECURSE VCPKG_INSTALLED_BINARIES_RELEASE ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*)

install(FILES
        ${VCPKG_INSTALLED_BINARIES_DEBUG}
        TYPE BIN
        CONFIGURATIONS Debug)
install(FILES
        ${VCPKG_INSTALLED_BINARIES_RELEASE}
        TYPE BIN
        CONFIGURATIONS Release RelWithDebInfo MinSizeRel)

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