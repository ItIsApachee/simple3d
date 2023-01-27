cmake_minimum_required(VERSION 3.16)

option(SIMPLE3D_BUILD_EXAMPLES "Build the Simple3D example programs" ON)
option(SIMPLE3D_USE_VCPKG "Use vcpkg for resolving ANGLE & zlib dependency" ON)

# FIXME
message(STATUS "CMAKE_TOOLCHAIN_FILE: ${CMAKE_TOOLCHAIN_FILE}")

message(STATUS "SIMPLE3D_BUILD_EXAMPLES value: ${SIMPLE3D_BUILD_EXAMPLES}")
message(STATUS "SIMPLE3D_USE_VCPKG value: ${SIMPLE3D_USE_VCPKG}")

if (SIMPLE3D_BUILD_EXAMPLES AND SIMPLE3D_USE_VCPKG)
  message(STATUS "simple3d is enabling vcpkg manifest mode with auto-install")
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

# resource setup
if (SIMPLE3D_BUILD_EXAMPLES)
  CPMAddPackage(
    NAME simple3d_resources
    VERSION 0.1
    URL https://github.com/ItIsApachee/simple3d/releases/download/v0.1/resources.zip
    DOWNLOAD_ONLY
  )
endif()

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
  install(FILES
          ${simple3d_SOURCE_DIR}/examples/imgui.ini
          TYPE BIN
          CONFIGURATIONS Debug Release RelWithDebInfo MinSizeRel)

  # add resources
  install(DIRECTORY ${simple3d_resources_SOURCE_DIR}/Minecraft_Grass_Block_OBJ
          TYPE BIN)

  if(SIMPLE3D_USE_VCPKG)
    message(STATUS "locating binaries of ANGLE and its dependencies...")
    file(GLOB_RECURSE VCPKG_INSTALLED_BINARIES_DEBUG ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/debug/bin/*)
    file(GLOB_RECURSE VCPKG_INSTALLED_BINARIES_RELEASE ${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin/*)
    message(STATUS "ANGLE debug: ${VCPKG_INSTALLED_BINARIES_DEBUG}")
    message(STATUS "ANGLE release: ${VCPKG_INSTALLED_BINARIES_RELEASE}")

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
