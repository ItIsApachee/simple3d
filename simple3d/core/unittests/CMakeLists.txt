# NB(apachee): Currently tests only run under native platform

add_executable(
    simple3d_core_unittests
    error_ut.cpp)

target_link_libraries(
    simple3d_core_unittests
    PRIVATE simple3d_core GTest::gtest_main)

set_property(TARGET simple3d_core_unittests PROPERTY CXX_STANDARD 20)

gtest_discover_tests(simple3d_core_unittests)