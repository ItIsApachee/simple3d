project(examples)

add_executable(examples::interactive_scene main.cc)
target_link_libraries(examples::interactive_scene simple3d simple3d-imgui)

add_executable(examples::interactive_model interactive_model.cc)
target_link_libraries(examples::interactive_model simple3d simple3d-imgui)