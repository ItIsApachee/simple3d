# NB(apachee): Core library must be compatible for all native platforms and emscripten,
# because it is used both in client library and web client implementation.

add_library(simple3d_core STATIC
    assert.cpp error.cpp)

target_link_libraries(simple3d_core PUBLIC glm::glm)

target_include_directories(simple3d_core SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

set_property(TARGET simple3d_core PROPERTY CXX_STANDARD 20)
# NB(apachee): For usage withing web client
set_property(TARGET simple3d_core PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")

if (DEFINED IS_MAIN_PROJECT AND IS_MAIN_PROJECT)
    add_subdirectory(unittests)
endif()
