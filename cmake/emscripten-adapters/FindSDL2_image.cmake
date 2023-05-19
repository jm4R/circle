add_library(SDL2_image_emscripten_builtin INTERFACE)

target_compile_options(SDL2_image_emscripten_builtin INTERFACE "-sUSE_SDL_IMAGE=2")
target_link_options(SDL2_image_emscripten_builtin INTERFACE "-sUSE_SDL_IMAGE=2")

add_library(SDL2_image::SDL2_image ALIAS SDL2_image_emscripten_builtin)
