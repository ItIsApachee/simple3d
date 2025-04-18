add_library(
    apachee_simple3d_web_client_gl STATIC
    renderer.cpp)

target_link_libraries(apachee_simple3d_web_client_gl PUBLIC apachee_simple3d_core apachee_simple3d_web_client_lib)
target_include_directories(apachee_simple3d_web_client_gl SYSTEM PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})

set_property(TARGET apachee_simple3d_web_client_gl PROPERTY CXX_STANDARD 20)
set_property(TARGET apachee_simple3d_web_client_gl PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")