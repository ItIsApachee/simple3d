add_library(simple3d_web_server STATIC server.cpp)

target_link_libraries(simple3d_web_server PUBLIC simple3d_core uWebSockets)
set_property(TARGET simple3d_web_server PROPERTY CXX_STANDARD 20)
target_include_directories(simple3d_web_server SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

add_dependencies(simple3d_web_server simple3d_web_client)

add_executable(simple3d_web_server_example program.cpp)
target_link_libraries(simple3d_web_server_example simple3d_web_server)
set_property(TARGET simple3d_web_server_example PROPERTY CXX_STANDARD 20)
target_include_directories(simple3d_web_server_example SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

install(
    TARGETS simple3d_web_server_example
    RUNTIME
    DESTINATION .
)
