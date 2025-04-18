# NB(apachee): Core library must be compatible for all native platforms and emscripten,
# because it is used both in client library and web client implementation.

file(GLOB_RECURSE SIMPLE3D_CORE_SOURCES *.cpp)

add_library(simple3d_core STATIC ${SIMPLE3D_CORE_SOURCES})

target_include_directories(simple3d_core SYSTEM PUBLIC ${SIMPLE3D_SOURCE_DIR})

set_property(TARGET simple3d_core PROPERTY CXX_STANDARD 20)
# NB(apachee): For usage withing web client
set_property(TARGET simple3d_core PROPERTY COMPILE_FLAGS "${EM_CFLAGS}")
