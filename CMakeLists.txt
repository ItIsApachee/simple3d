cmake_minimum_required(VERSION 3.13)
project(simple3d)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(examples)

set(SIMPLE3D_BUILD_INTERNAL_DOCS YES CACHE STRING "specify whether you need documentation of private API")
message(${CMAKE_CURRENT_SOURCE_DIR})
find_package(Doxygen)
if (DOXYGEN_FOUND)
    configure_file(
        Doxyfile.in
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
    )
    message("DBG ${CMAKE_CURRENT_BINARY_DIR}")
    add_custom_target(
        doc
        ${DOXYGEN_EXECUTABLE}
        ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Target to generate docs" VERBATIM
    )
endif()