add_library(
    simple3d_web_client_gl STATIC
    renderer.cpp)

target_link_libraries(simple3d_web_client_gl PUBLIC simple3d_core simple3d_web_client_lib)
target_include_directories(simple3d_web_client_gl SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

set_property(TARGET simple3d_web_client_gl PROPERTY CXX_STANDARD 20)
set_property(TARGET simple3d_web_client_gl PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")