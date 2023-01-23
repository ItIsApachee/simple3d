# Library setup
add_subdirectory(graphics/internal)

# simple3d-imgui setup
add_subdirectory(imgui)

# set(simple3d_SOURCES simple3d.cc)
# set(simple3d_SOURCES ${simple3d_SOURCES} shader/shader.cc)

# src/context/
set(simple3d_SOURCES ${simple3d_SOURCES} context/context.cc context/window.cc
    context/input.cc)

# src/graphics/
set(simple3d_SOURCES ${simple3d_SOURCES} graphics/scene.cc graphics/view.cc
    graphics/model_handle.cc graphics/camera.cc graphics/shader_storage.cc)

# /src/graphics/internal/
set(simple3d_SOURCES ${simple3d_SOURCES}
    graphics/internal/misc.cc
    graphics/internal/vertex_array_object.cc
    graphics/internal/vertex_buffer_object.cc
    graphics/internal/element_buffer_object.cc
    graphics/internal/gles_shader.cc
    graphics/internal/vertex.cc)

# /src/graphics/models/
set(simple3d_SOURCES ${simple3d_SOURCES} graphics/models/cuboid.cc
    graphics/models/sphere.cc graphics/models/model_shader.cc
    graphics/models/textured_model_shader.cc graphics/models/model.cc
    graphics/models/primitive_renderer.cc)

# /src/utils/
set(simple3d_SOURCES ${simple3d_SOURCES}
    utils/fps_camera.cc)

add_library(simple3d STATIC ${simple3d_SOURCES})

target_link_libraries(
    simple3d
    PUBLIC glfw glad glm
    PRIVATE assimp stb-image
)

target_include_directories(
    simple3d
    PRIVATE ${PROJECT_BINARY_DIR}/include/
    PUBLIC ${PROJECT_SOURCE_DIR}/include/
)