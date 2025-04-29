set(SIMPLE3D_MAKE_EXECUTABLE "make" CACHE FILEPATH "")
set(
    SIMPLE3D_USOCKETS_OPTIONS
    "WITH_BORINGSSL=1"
    "WITH_LTO=1"
    CACHE STRING "")

# TODO(apachee): Ensure this lib works on Windows.
# TODO(apachee): In case Windows build proves to be difficult, then apply
# similar to web client technique by making standalone binary which proxies
# communication with web client.

list(JOIN "${SIMPLE3D_USOCKETS_OPTIONS}" " " SIMPLE3D_USOCKETS_OPTIONS_PROCESSED)

FetchContent_Declare(
    uSockets_content
    GIT_REPOSITORY https://github.com/uNetworking/uSockets
    GIT_TAG v0.8.8
    GIT_SHALLOW ON
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(uSockets_content)

if (NOT usockets_content_POPULATED)
    message(FATAL_ERROR "uSockets not populated")
endif()

file(GLOB_RECURSE SOURCES ${usockets_content_SOURCE_DIR}/src/*.c)
add_library(uSockets ${SOURCES})
target_include_directories(uSockets PUBLIC ${usockets_content_SOURCE_DIR}/src)
target_compile_definitions(uSockets PRIVATE LIBUS_NO_SSL)
set_property(TARGET uSockets PROPERTY C_STANDARD 11)
set_property(TARGET uSockets PROPERTY COMPILE_WARNING_AS_ERROR OFF)

FetchContent_Declare(
    uWebSockets_content
    GIT_REPOSITORY https://github.com/uNetworking/uWebSockets
    GIT_TAG v20.74.0
    GIT_SHALLOW ON
    GIT_SUBMODULES ""
)
FetchContent_MakeAvailable(uWebSockets_content)

execute_process(
    COMMAND ${CMAKE_COMMAND} -E make_directory ${SIMPLE3D_BINARY_DIR}/uWebSockets/include
    COMMAND ${CMAKE_COMMAND} -E create_symlink ${uwebsockets_content_SOURCE_DIR}/src ${SIMPLE3D_BINARY_DIR}/uWebSockets/include/uWebSockets)

if (NOT uwebsockets_content_POPULATED)
    message(FATAL_ERROR "uWebSockets not populated")
endif()

add_library(uWebSockets INTERFACE)
target_include_directories(uWebSockets SYSTEM INTERFACE ${SIMPLE3D_BINARY_DIR}/uWebSockets/include)
target_link_libraries(uWebSockets INTERFACE uSockets)
# target_link_libraries(uWebSockets INTERFACE uSockets ${ZLIB_LIBRARIES})
target_compile_definitions(uWebSockets INTERFACE UWS_NO_ZLIB)
target_compile_options(uWebSockets INTERFACE -Wno-deprecated-declarations)
set_property(TARGET uWebSockets PROPERTY CXX_STANDARD 17)

add_subdirectory(server)
