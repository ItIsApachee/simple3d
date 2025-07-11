add_library(
    simple3d_web_client_lib STATIC

    webgl2/ebo.cpp
    webgl2/helpers.cpp
    webgl2/shader_program.cpp
    webgl2/uniform_id.cpp
    webgl2/vao.cpp
    webgl2/vbo.cpp
    webgl2/webgl.cpp

    graphics/camera.cpp
    graphics/config.cpp
    graphics/fps_camera.cpp
    graphics/model_shader.cpp

    io.cpp)

target_link_libraries(simple3d_web_client_lib
    PUBLIC simple3d_core simple3d_web_client_lib_shader_sources
    PUBLIC glm::glm)
target_include_directories(simple3d_web_client_lib SYSTEM PUBLIC "${SIMPLE3D_SOURCE_DIR}" "${SIMPLE3D_WEB_CLIENT_BINARY_DIR}/include")

target_link_options(simple3d_web_client_lib PUBLIC "SHELL:-lwebsocket.js")

set_property(TARGET simple3d_web_client_lib  PROPERTY CXX_STANDARD 20)
set_property(TARGET simple3d_web_client_lib PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")

add_subdirectory(graphics)
