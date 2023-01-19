project(stb)

add_library(stb-image STATIC stb_image.cc)
target_include_directories(stb-image PUBLIC include/)