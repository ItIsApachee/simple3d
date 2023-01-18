project(simple3d-imgui)

add_library(simple3d-imgui STATIC imgui.cc)

target_link_libraries(simple3d-imgui
    PUBLIC simple3d glfw imgui imgui-glfw imgui-gles3)

target_include_directories(simple3d-imgui
    PUBLIC ${simple3d_SOURCE_DIR}/include/)