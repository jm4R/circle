#pragma once

#include <cstdint>
#include <variant>
#include <cstddef>

struct SDL_Window;
struct SDL_Renderer;
struct SDL_Texture;
using TTF_Font = struct _TTF_Font;

namespace circle::engine {

struct context
{
    SDL_Renderer* renderer{};
    SDL_Texture* texture{};
};

struct window_context : public context
{
    SDL_Window* window{};
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

using unit = int;

struct size
{
    unit w, h;
};

// base
void init_engine();
void destroy_engine() noexcept;

bool is_init(context& ctx) noexcept;

// window
long window_init(window_context& ctx, unsigned w, unsigned h);
void window_destroy(window_context& ctx) noexcept;
void window_show(window_context& ctx, bool show) noexcept;

// texture
void texture_init(context& ctx, window_context& window, unit w,
                  unit h) noexcept;
void texture_destroy(context& ctx) noexcept;
size texture_size(context& ctx) noexcept;

// font
void font_init(font_context& font, const char* path, unit size) noexcept;
void font_init_from_memory(font_context& font, const std::byte* data,
                           std::size_t data_length, unit size) noexcept;
font_style font_set_style(font_context& font, font_style flags) noexcept;
void font_destroy(font_context& font) noexcept;
unit font_get_line_height(font_context& font);
unit font_get_line_skip(font_context& font);
unit font_get_text_width(font_context& font, const char* text);
const char* font_get_family_name(font_context& font);
const char* font_get_style_name(font_context& font);
// event handling
struct resize_event
{
    long window_id; // returned by window_init
    int width;
    int height;
};
struct quit_event{};
using event = std::variant<resize_event, quit_event>;
using event_handler = void (*)(void*, const event&);
event wait_for_event();
void post_event();
void set_event_handler(window_context& ctx, event_handler handler,
                       void* user_data = nullptr) noexcept;

// rendering
void flush(context& ctx) noexcept;
void clear(context& ctx) noexcept;
void grab_pixels(context& ctx, std::byte* data, int w, int h) noexcept;
void set_color(context& ctx, std::uint8_t r, std::uint8_t g, std::uint8_t b,
               std::uint8_t a) noexcept;
void draw_rectangle(context& ctx, unit x, unit y, unit w, unit h) noexcept;
void draw_rectangle_filled(context& ctx, unit x, unit y, unit w,
                           unit h) noexcept;
void draw_rectangle_filled_rounded(context& ctx, unit x, unit y, unit w, unit h,
                                   unit r, bool antialiasing) noexcept;
void draw_gradient(context& ctx, unit x, unit y, unit w, unit h,
                   std::uint8_t r1, std::uint8_t g1, std::uint8_t b1,
                   std::uint8_t a1, std::uint8_t r2, std::uint8_t g2,
                   std::uint8_t b2, std::uint8_t a2, bool vertical) noexcept;
void draw_texture(context& dst, context& src) noexcept;
void draw_texture(context& dst, context& src, unit dstx, unit dsty, unit dstw,
                  unit dsth) noexcept;
void apply_alpha_mask(context& dst, context& src) noexcept;
void draw_text_line(context& ctx, const font_context& font, const char* text,
                    unit x, unit y, unit w, unit h) noexcept;

} // namespace circle::engine
