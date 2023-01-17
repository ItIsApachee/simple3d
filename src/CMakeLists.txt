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

# GLM setup
FetchContent_Declare(
    glm
    GIT_REPOSITORY https://github.com/g-truc/glm.git
    GIT_TAG 0.9.9.8
)

FetchContent_GetProperties(glm)
if (NOT glm_POPULATED)
    FetchContent_Populate(glm)
endif()

add_subdirectory(${glm_SOURCE_DIR} ${glm_BINARY_DIR})

# Remove FORCE (present for debug purpose)
# FIXME: ANGLE relies on libGLESv2.dll (this exact filename), but libGLESv2_ANGLE.dll seems more appropriate
set(SIMPLE3D_EGL_LIBRARY "libEGL.dll" CACHE STRING "path to EGL library for GLFW" FORCE)
set(SIMPLE3D_GLESv2_LIBRARY "libGLESv2.dll" CACHE STRING "path to GLESv2 library for GLFW" FORCE)
target_compile_definitions(
    glfw
    PRIVATE _GLFW_EGL_LIBRARY="${SIMPLE3D_EGL_LIBRARY}"
    PRIVATE _GLFW_GLESV2_LIBRARY="${SIMPLE3D_GLESv2_LIBRARY}"
)

# Library setup
add_subdirectory(graphics/internal)

# set(simple3d_SOURCES simple3d.cc)
# set(simple3d_SOURCES ${simple3d_SOURCES} shader/shader.cc)

# src/context/
set(simple3d_SOURCES ${simple3d_SOURCES} context/context.cc context/window.cc context/input.cc)
# src/graphics/
set(simple3d_SOURCES ${simple3d_SOURCES} graphics/scene.cc graphics/view.cc
    graphics/model.cc graphics/camera.cc graphics/shader_storage.cc)

# /src/graphics/internal
set(simple3d_SOURCES ${simple3d_SOURCES}
    graphics/internal/misc.cc
    graphics/internal/vertex_array_object.cc
    graphics/internal/vertex_buffer_object.cc
    graphics/internal/element_buffer_object.cc
    graphics/internal/gles_shader.cc
    graphics/internal/vertex.cc)

# /src/graphics/models
set(simple3d_SOURCES ${simple3d_SOURCES}
    graphics/models/cuboid.cc
    graphics/models/model_shader.cc)

add_library(simple3d STATIC ${simple3d_SOURCES})

target_link_libraries(
    simple3d
    PUBLIC glfw
    PUBLIC glad
    PUBLIC glm
)

target_include_directories(
    simple3d
    # use "" to include
    # BEFORE PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}
    PRIVATE ${CMAKE_BINARY_DIR}/include
    PUBLIC ${CMAKE_SOURCE_DIR}/include
)