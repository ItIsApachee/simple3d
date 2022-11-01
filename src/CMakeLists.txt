include(FetchContent)

FetchContent_Declare(
    glfw
    GIT_REPOSITORY https://github.com/glfw/glfw.git
    GIT_TAG 3.3.8
    # SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/thirdparty/
)

FetchContent_GetProperties(glfw)
if(NOT glfw_POPULATED)
    FetchContent_Populate(glfw)
endif()
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(${glfw_SOURCE_DIR} ${glfw_BINARY_DIR})

find_package(OpenGL REQUIRED)

add_executable(app main.cpp)
target_link_libraries(app glfw)
target_link_libraries(app OpenGL::GL)