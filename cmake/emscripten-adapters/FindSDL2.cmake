add_library(SDL2_emscripten_builtin INTERFACE)

target_compile_options(SDL2_emscripten_builtin INTERFACE "-sUSE_SDL=2")
target_link_options(SDL2_emscripten_builtin INTERFACE "-sUSE_SDL=2")

add_library(SDL2::SDL2 ALIAS SDL2_emscripten_builtin)
