#pragma once

#include <cstdint>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
using TTF_Font = struct _TTF_Font;
union SDL_Event;

namespace circle::sdl {

struct context
{
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
};

struct window_context : public context
{
    SDL_Window* window{};
    std::uint32_t id;
};

struct font_context
{
    TTF_Font* font{};
};

using event = SDL_Event;

} // namespace circle::sdl