file(READ fragment.glsl SIMPLE3D_SHADER_FRAGMENT_GLSL)
file(READ vertex.glsl SIMPLE3D_SHADER_VERTEX_GLSL)

configure_file(
    shader_source.h.in
    ${CMAKE_BINARY_DIR}/include/simple3d/shader/shader_source.h
)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS vertex.glsl)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS fragment.glsl)