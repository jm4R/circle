#pragma once

#include <circle/utils/sdl_utils.hpp>

#include <SDL2/SDL_events.h>

#include <cstddef>
#include <cstdint>

namespace circle::sdl {

struct size
{
    int w, h;
};

// base
void init_engine();
void destroy_engine() noexcept;
bool is_init(context& ctx) noexcept;

// events
void event_wait(event& ev);
void event_push(event ev);

// window
void window_init(window_context& ctx, unsigned w, unsigned h);
void window_destroy(window_context& ctx) noexcept;
void window_show(window_context& ctx, bool show) noexcept;

// texture
void texture_init(context& ctx, context& src_context, int w, int h) noexcept;
void texture_destroy(context& ctx) noexcept;
size texture_size(context& ctx) noexcept;

// image
void image_init(context& ctx, context& src_context, const char* file_path) noexcept;

// font
void font_init(font_context& font, const char* path, int size) noexcept;
void font_init_from_memory(font_context& font, const std::byte* data,
                           std::size_t data_length, int size) noexcept;
font_style font_set_style(font_context& font, font_style flags) noexcept;
void font_destroy(font_context& font) noexcept;
int font_get_line_height(font_context& font);
int font_get_line_skip(font_context& font);
int font_get_text_width(font_context& font, const char* text);
const char* font_get_family_name(font_context& font);
const char* font_get_style_name(font_context& font);

// rendering
void flush(context& ctx) noexcept;
void clear(context& ctx) noexcept;
void grab_pixels(context& ctx, std::byte* data, int w, int h) noexcept;
void set_color(context& ctx, std::uint8_t r, std::uint8_t g, std::uint8_t b,
               std::uint8_t a) noexcept;
void draw_rectangle(context& ctx, int x, int y, int w, int h) noexcept;
void draw_rectangle_filled(context& ctx, int x, int y, int w, int h) noexcept;
void draw_rectangle_filled_rounded(context& ctx, int x, int y, int w, int h,
                                   int r, bool antialiasing) noexcept;
void draw_gradient(context& ctx, int x, int y, int w, int h, std::uint8_t r1,
                   std::uint8_t g1, std::uint8_t b1, std::uint8_t a1,
                   std::uint8_t r2, std::uint8_t g2, std::uint8_t b2,
                   std::uint8_t a2, bool vertical) noexcept;
void draw_texture(context& dst, context& src) noexcept;
void draw_texture(context& dst, context& src, int dstx, int dsty, int dstw,
                  int dsth) noexcept;
void apply_alpha_mask(context& dst, context& src) noexcept;
void draw_text_line(context& ctx, const font_context& font, const char* text,
                    int x, int y, int w, int h) noexcept;

using timer_clb = std::uint32_t(*)(std::uint32_t msec, void* param);
SDL_TimerID start_timer(std::uint32_t msec, timer_clb clb, void* param);
bool stop_timer(SDL_TimerID id);

} // namespace circle::sdl
