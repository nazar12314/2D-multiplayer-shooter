message("-- Linking SDL2_image")
add_library(SDL2_image STATIC IMPORTED SDL2_image)
set_target_properties(SDL2_image PROPERTIES LINKER_LANGUAGE CXX)