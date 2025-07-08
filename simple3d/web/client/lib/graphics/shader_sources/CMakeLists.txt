file(READ model/vertex.glsl SIMPLE3D_MODEL_SHADER_VERTEX_GLSL)
file(READ model/fragment.glsl SIMPLE3D_MODEL_SHADER_FRAGMENT_GLSL)

file(READ textured/fragment.glsl SIMPLE3D_SHADER_TEXTURED_FRAGMENT_GLSL)
file(READ textured/vertex.glsl SIMPLE3D_SHADER_TEXTURED_VERTEX_GLSL)

configure_file(
    shader_sources.cpp.in
    "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/include/simple3d/web/client/lib/graphics/shader_sources/shader_sources.cpp"
)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS shader_sources.h.in)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS model/vertex.glsl)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS model/fragment.glsl)

set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS textured/vertex.glsl)
set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS textured/fragment.glsl)

add_library(simple3d_web_client_lib_shader_sources STATIC
    "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/include/simple3d/web/client/lib/graphics/shader_sources/shader_sources.cpp")

target_include_directories(simple3d_web_client_lib_shader_sources SYSTEM PUBLIC "${SIMPLE3D_SOURCE_DIR}" "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/include")

set_property(TARGET simple3d_web_client_lib_shader_sources PROPERTY CXX_STANDARD 20)
set_property(TARGET simple3d_web_client_lib_shader_sources PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")
