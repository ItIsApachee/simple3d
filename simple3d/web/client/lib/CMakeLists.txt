add_library(
    simple3d_web_client_lib STATIC
    webgl2/webgl.cpp)

target_link_libraries(simple3d_web_client_lib PUBLIC simple3d_core)
target_include_directories(simple3d_web_client_lib SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

set_property(TARGET simple3d_web_client_lib  PROPERTY CXX_STANDARD 20)
set_property(TARGET simple3d_web_client_lib PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")