# NB(apachee): Core library must be compatible for all native platforms and emscripten,
# because it is used both in client library and frontend implementation.

file(GLOB_RECURSE APACHEE_SIMPLE3D_CORE_SOURCES *.cpp)

add_library(apachee_simple3d_core STATIC ${APACHEE_SIMPLE3D_CORE_SOURCES})
target_include_directories(apachee_simple3d_core SYSTEM PUBLIC ${APACHEE_SIMPLE3D_SOURCE_DIR})
set_property(TARGET apachee_simple3d_core PROPERTY CXX_STANDARD 20)
