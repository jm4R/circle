add_library(SDL2_ttf_emscripten_builtin INTERFACE)

target_compile_options(SDL2_ttf_emscripten_builtin INTERFACE "-sUSE_SDL_TTF=2")
target_link_options(SDL2_ttf_emscripten_builtin INTERFACE "-sUSE_SDL_TTF=2")

add_library(SDL2_ttf::SDL2_ttf ALIAS SDL2_ttf_emscripten_builtin)
