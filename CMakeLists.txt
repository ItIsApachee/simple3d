cmake_minimum_required(VERSION 3.21)

option(SIMPLE3D_BUILD_EXAMPLES "Build the Simple3D example programs" ON)
option(SIMPLE3D_USE_VCPKG "Use vcpkg for resolving ANGLE & zlib dependency" ON)

if (SIMPLE3D_BUILD_EXAMPLES AND SIMPLE3D_USE_VCPKG)
  set(VCPKG_MANIFEST_MODE ON)
  set(VCPKG_MANIFEST_INSTALL ON)
endif()

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

# find_package(ZLIB REQUIRED)
# find_package(unofficial-angle CONFIG REQUIRED)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(examples)

# install configuration for examples
if (SIMPLE3D_BUILD_EXAMPLES)
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

  if(SIMPLE3D_USE_VCPKG)
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
   endif()
endif()

option(SIMPLE3D_BUILD_DOCS "Generate target for building docs" ON)
if(SIMPLE3D_BUILD_DOCS)
  find_package(Doxygen REQUIRED)

  # Doxygen configuration
  set(SIMPLE3D_DOCS_OUTPUT_LANGUAGE "Russian" CACHE STRING "Documentation language")
  set(SIMPLE3D_DOXYGEN_INPUT "${CMAKE_SOURCE_DIR}/include/simple3d/")

  # set(SIMPLE3D_BUILD_INTERNAL_DOCS ON CACHE BOOL "specify whether you need documentation of private API")
  # if (${SIMPLE3D_BUILD_INTERNAL_DOCS})
  #     set(SIMPLE3D_DOXYGEN_INPUT ${SIMPLE3D_DOXYGEN_INPUT} "${CMAKE_SOURCE_DIR}/src/")
  # endif()

  # Doxygen format for list of directories
  list(JOIN SIMPLE3D_DOXYGEN_INPUT " " SIMPLE3D_DOXYGEN_INPUT)

  configure_file(
      Doxyfile.in
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
  )
  add_custom_target(
      simple3d-docs
      ${DOXYGEN_EXECUTABLE}
      ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Target to generate docs" VERBATIM)
  set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS Doxyfile.in)
endif()
