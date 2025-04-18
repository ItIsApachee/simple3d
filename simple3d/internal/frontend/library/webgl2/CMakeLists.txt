# file(READ shader_sources/default/fragment.glsl SIMPLE3D_SHADER_DEFAULT_FRAGMENT_GLSL)
# file(READ shader_sources/default/vertex.glsl SIMPLE3D_SHADER_DEFAULT_VERTEX_GLSL)

# file(READ shader_sources/textured/fragment.glsl SIMPLE3D_SHADER_TEXTURED_FRAGMENT_GLSL)
# file(READ shader_sources/textured/vertex.glsl SIMPLE3D_SHADER_TEXTURED_VERTEX_GLSL)

# configure_file(
#     shader_source.h.in
#     ${simple3d_BINARY_DIR}/include/simple3d/graphics/internal/shader_source.h
# )

# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_source.h.in)

# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_sources/default/vertex.glsl)
# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_sources/default/fragment.glsl)

# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_sources/textured/vertex.glsl)
# set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_sources/textured/fragment.glsl)

# FIXME(apachee): Fix this code for new code style and new project structure
# FIXME(apachee): Since emscripten statically links all required headers, code might need some reworking.
# FIXME(apachee): Get rid of GLFW? Create loader?