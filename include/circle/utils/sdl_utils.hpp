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

enum font_style
{
    font_style_normal = 0x00,
    font_style_bold = 0x01,
    font_style_italic = 0x02,
    font_style_underline = 0x04,
    font_style_strikethrough = 0x08
};

using event = SDL_Event;

} // namespace circle::sdl