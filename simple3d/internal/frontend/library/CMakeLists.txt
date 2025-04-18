add_library(
    apachee_simple3d_frontend_lib STATIC
    webgl2/webgl.cpp)

target_link_libraries(apachee_simple3d_frontend_lib PUBLIC apachee_simple3d_core)
target_include_directories(apachee_simple3d_frontend_lib SYSTEM PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})

set_property(TARGET apachee_simple3d_frontend_lib  PROPERTY CXX_STANDARD 20)