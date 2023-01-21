project(simple3d-examples)

add_executable(simple3d-interactive-scene interactive_scene.cc)
target_link_libraries(simple3d-interactive-scene simple3d simple3d-imgui)

add_executable(simple3d-interactive-model interactive_model.cc)
target_link_libraries(simple3d-interactive-model simple3d simple3d-imgui)