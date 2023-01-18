# imgui setup
set(imgui_SOURCES imgui.cpp imgui_demo.cpp imgui_draw.cpp
    imgui_tables.cpp imgui_widgets.cpp)
list(TRANSFORM imgui_SOURCES PREPEND ${imgui_SOURCE_DIR}/)

add_library(imgui STATIC ${imgui_SOURCES})

target_include_directories(imgui PUBLIC ${imgui_SOURCE_DIR})

# imgui-glfw setup
set(imgui-glfw_SOURCES backends/imgui_impl_glfw.cpp backends/imgui_impl_glfw.h)
list(TRANSFORM imgui-glfw_SOURCES PREPEND ${imgui_SOURCE_DIR}/)

add_library(imgui-glfw STATIC ${imgui-glfw_SOURCES})

target_link_libraries(imgui-glfw PUBLIC imgui glfw)

target_include_directories(imgui-glfw PUBLIC ${imgui_SOURCE_DIR})

# imgui-gles3 setup
set(imgui-gles3_SOURCES imgui-gles3/imgui_impl_gles3.cpp)

add_library(imgui-gles3 STATIC ${imgui-gles3_SOURCES})

target_link_libraries(imgui-gles3 PUBLIC imgui glad)

target_include_directories(imgui-gles3 PUBLIC ${imgui_SOURCE_DIR} .)