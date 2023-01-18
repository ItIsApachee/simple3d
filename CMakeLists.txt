cmake_minimum_required(VERSION 3.14)
project(simple3d)

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(thirdparty)
add_subdirectory(src)
add_subdirectory(examples)

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