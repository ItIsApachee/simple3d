include(FetchContent)

# GLFW setup
FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
endif()
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

set(SIMPLE3D_EGL_LIBRARY "libEGL_ANGLE.dll" CACHE STRING "path to EGL library for GLFW")
set(SIMPLE3D_GLESv2_LIBRARY "libGLESv2_ANGLE.dll" CACHE STRING "path to GLESv2 library for GLFW")
target_compile_definitions(
    glfw
    PRIVATE _GLFW_EGL_LIBRARY="${SIMPLE3D_EGL_LIBRARY}"
    PRIVATE _GLFW_GLESV2_LIBRARY="${SIMPLE3D_GLESv2_LIBRARY}"
)

# Library setup
add_library(simple3d STATIC simple3d.cc context/context.cc)

target_link_libraries(simple3d PUBLIC glfw)

target_include_directories(simple3d PUBLIC ${CMAKE_SOURCE_DIR}/include)
# not sure if it should be public
target_include_directories(simple3d PRIVATE ${CMAKE_SOURCE_DIR}/thirdparty)