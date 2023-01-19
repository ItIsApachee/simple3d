include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

add_subdirectory(glad)

CPMAddPackage(
    imgui
    GIT_REPOSITORY https://github.com/ocornut/imgui.git
    GIT_TAG master
    DOWNLOAD_ONLY
)

add_subdirectory(imgui)

add_subdirectory(stb)