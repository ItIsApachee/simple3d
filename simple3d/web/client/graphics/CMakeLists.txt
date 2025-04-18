add_library(
    apachee_simple3d_web_client_graphics STATIC
    renderer.cpp)

target_link_libraries(apachee_simple3d_web_client_graphics PUBLIC apachee_simple3d_core apachee_simple3d_web_client_lib)
target_include_directories(apachee_simple3d_web_client_graphics SYSTEM PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})

set_property(TARGET apachee_simple3d_web_client_graphics PROPERTY CXX_STANDARD 20)
set_property(TARGET apachee_simple3d_web_client_graphics PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")