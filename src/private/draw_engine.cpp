#include <circle/private/draw_engine.hpp>

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#define SDL_LINE_DRAWING_BUG (SDL_MAJOR_VERSION == 2 && SDL_MINOR_VERSION == 0 && SDL_PATCHLEVEL < 14)
#if SDL_LINE_DRAWING_BUG
#warning "There is some serious bug in used SDL version"
#endif

#include <algorithm>
#include <cassert>
#include <functional>
#include <unordered_map>

namespace circle::engine {

namespace {

struct events_map
{
    bool registered{};
    struct handler_data
    {
        event_handler fun;
        void* data;
    };

    std::unordered_map<Uint32, handler_data> windows_handlers;
};

SDL_Renderer* renderer(context& ctx)
{
    SDL_SetRenderTarget(ctx.renderer, ctx.texture);
    return ctx.renderer;
}

} // namespace

void init_engine()
{
    const auto res = SDL_Init(SDL_INIT_VIDEO);
    if (res < 0)
        throw "Could not init SDL";

    const auto res2 = TTF_Init();
    if (res2 < 0)
        throw "Could not init SDL TTF module";
}

void destroy_engine() noexcept
{
    TTF_Quit();
    SDL_Quit();
}

bool is_init(context& ctx) noexcept
{
    return ctx.renderer != nullptr;
}

long window_init(window_context& ctx, unsigned w, unsigned h)
{
    ctx.window = SDL_CreateWindow("circle", SDL_WINDOWPOS_UNDEFINED,
                                  SDL_WINDOWPOS_UNDEFINED, w, h,
                                  SDL_WINDOW_HIDDEN | SDL_WINDOW_RESIZABLE);

    int val;
    if (!ctx.window)
        throw "Could not create SDL window";

    ctx.renderer = SDL_CreateRenderer(
        ctx.window, -1, SDL_RENDERER_ACCELERATED /*TODO software fallback*/);

    SDL_RendererInfo i;
    SDL_GetRendererInfo(ctx.renderer, &i);

    if (!ctx.renderer)
        throw "Could not create SDL renderer";

    SDL_SetRenderDrawBlendMode(ctx.renderer, SDL_BLENDMODE_BLEND);

    return SDL_GetWindowID(ctx.window);
}

void window_destroy(window_context& ctx) noexcept
{
    SDL_DestroyRenderer(ctx.renderer);
    SDL_DestroyWindow(ctx.window);
    ctx.renderer = nullptr;
}

void window_show(window_context& ctx, bool show) noexcept
{
    if (show)
        SDL_ShowWindow(ctx.window);
    else
        SDL_HideWindow(ctx.window);
}

void texture_init(context& ctx, window_context& window, unit w, unit h) noexcept
{
    ctx.renderer = window.renderer;
    ctx.texture = SDL_CreateTexture(ctx.renderer, SDL_PIXELFORMAT_RGBA32,
                                    SDL_TEXTUREACCESS_TARGET, w, h);
    SDL_SetTextureBlendMode(ctx.texture, SDL_BLENDMODE_BLEND);
}

void texture_destroy(context& ctx) noexcept
{
    SDL_DestroyTexture(ctx.texture);
    ctx.renderer = nullptr;
}

size texture_size(context &ctx) noexcept
{
    size res{};
    Uint32 ignored;
    int ignored2;
    SDL_QueryTexture(ctx.texture, &ignored, &ignored2, &res.w, &res.h);
    return res;
}

void font_init(font_context& font, const char* path, unit size) noexcept
{
    font.font = TTF_OpenFont(path, size);
}

void font_init_from_memory(font_context& font, const std::byte* data,
                           std::size_t data_length, unit size) noexcept
{
    const auto memory = SDL_RWFromConstMem(data, data_length);
    font.font = TTF_OpenFontRW(memory, true, size);
}

void font_destroy(font_context& font) noexcept
{
    TTF_CloseFont(font.font);

    font = font_context{};
}

font_style font_set_style(font_context& font, font_style flags) noexcept
{
    TTF_SetFontStyle(font.font, flags);
    return static_cast<font_style>(TTF_GetFontStyle(font.font));
}

unit font_get_line_height(font_context& font)
{
    return TTF_FontHeight(font.font);
}

unit font_get_line_skip(font_context& font)
{
    return TTF_FontLineSkip(font.font);
}

unit font_get_text_width(font_context& font, const char* text)
{
    int w{};
    int h{};
    TTF_SizeUTF8(font.font, text, &w, &h);
    return w;
}

const char* font_get_family_name(font_context& font)
{
    return TTF_FontFaceFamilyName(font.font);
}

const char* font_get_style_name(font_context& font)
{
    return TTF_FontFaceStyleName(font.font);
}

void set_event_handler(window_context& ctx,
                       void (*handler)(void*, const event&),
                       void* user_data) noexcept
{
    static events_map map;

    map.windows_handlers[SDL_GetWindowID(ctx.window)] = {handler, user_data};

    if (!map.registered)
    {
        map.registered = true;
        SDL_AddEventWatch(
            [](void* data, SDL_Event* ev) {
                if (!ev)
                    return 0;
                if (ev->type == SDL_WINDOWEVENT)
                {
                    const auto it =
                        map.windows_handlers.find(ev->window.windowID);
                    if (it == map.windows_handlers.end())
                        return 0;
                    switch (ev->window.event)
                    {
                    case SDL_WINDOWEVENT_RESIZED:
                    case SDL_WINDOWEVENT_SIZE_CHANGED:
                        it->second.fun(
                            it->second.data,
                            resize_event{ev->window.windowID, ev->window.data1, ev->window.data2});
                        break;
                    case SDL_WINDOWEVENT_SHOWN:
                    case SDL_WINDOWEVENT_HIDDEN:
                    case SDL_WINDOWEVENT_EXPOSED:
                    case SDL_WINDOWEVENT_MOVED:
                    case SDL_WINDOWEVENT_MINIMIZED:
                    case SDL_WINDOWEVENT_MAXIMIZED:
                    case SDL_WINDOWEVENT_RESTORED:
                    case SDL_WINDOWEVENT_ENTER:
                    case SDL_WINDOWEVENT_LEAVE:
                    case SDL_WINDOWEVENT_FOCUS_GAINED:
                    case SDL_WINDOWEVENT_FOCUS_LOST:
                    case SDL_WINDOWEVENT_CLOSE:
                    case SDL_WINDOWEVENT_TAKE_FOCUS:
                    case SDL_WINDOWEVENT_HIT_TEST:
                        break;
                    }
                }

                return 0;
            },
            user_data);
    }
}

void flush(context& ctx) noexcept
{
    SDL_RenderPresent(renderer(ctx));
}

void clear(context& ctx) noexcept
{
    SDL_RenderClear(renderer(ctx));
}

void grab_pixels(context& ctx, std::byte* data, int w, int h) noexcept
{
    int res = SDL_RenderReadPixels(renderer(ctx), nullptr,
                                   SDL_PIXELFORMAT_RGBA32, data, w * 4);
    assert(res == 0);
}

void set_color(context& ctx, std::uint8_t r, std::uint8_t g, std::uint8_t b,
               std::uint8_t a) noexcept
{
    SDL_SetRenderDrawColor(renderer(ctx), r, g, b, a);
}

void draw_rectangle(context& ctx, unit x, unit y, unit w, unit h) noexcept
{
    SDL_Rect rect{x, y, w, h};
    SDL_RenderDrawRect(renderer(ctx), &rect);
}

void draw_rectangle_filled(context& ctx, unit x, unit y, unit w,
                           unit h) noexcept
{
    SDL_Rect rect{x, y, w, h};
    SDL_RenderFillRect(renderer(ctx), &rect);
}

void draw_rectangle_filled_rounded(context& ctx, unit x, unit y, unit w, unit h,
                                   unit r, bool antialiasing) noexcept
{
    assert(w >= 0);
    assert(h >= 0);
    assert(r >= 0);
    if (r > w / 2)
        r = w / 2;
    if (r > h / 2)
        r = h / 2;

    if (r <= 0)
        return draw_rectangle_filled(ctx, x, y, w, h);

    auto ren = renderer(ctx);
    draw_rectangle_filled(ctx, x + r, y, w - 2 * r, h);
    draw_rectangle_filled(ctx, x, y + r, w, h - 2 * r);

    unit ly = r;
    unit lx;

    std::uint8_t cr, cg, cb, ca;
    SDL_GetRenderDrawColor(ctx.renderer, &cr, &cg, &cb, &ca);

    auto draw_part = [&] {
        SDL_RenderDrawLine(ren, x + w - r + lx - 1, y + r - ly,
                           x + w - r + lx - 1, y + h - r + ly - 1);
        SDL_RenderDrawLine(ren, x + r - ly, y + r - lx, x + w - r + ly - 1,
                           y + r - lx);
        SDL_RenderDrawLine(ren, x + r - ly, y + h - r + lx - 1,
                           x + w - r + ly - 1, y + h - r + lx - 1);
        SDL_RenderDrawLine(ren, x + r - lx, y + r - ly, x + r - lx,
                           y + h - r + ly - 1);
    };

    for (lx = 0; lx <= ly; ++lx)
    {
        if (lx * lx + ly * ly > r * r)
            ly--;

        SDL_SetRenderDrawColor(ren, cr, cg, cb, ca);

        draw_part();

        if (!antialiasing)
            continue;

        int a1 = lx * lx + (ly + 1) * (ly + 1) - r * r;
        int a2 = lx * lx + ly * ly - r * r;
        if (a2 == 0)
            continue;
        float a = -a2;
        a /= a1 - a2;
        SDL_SetRenderDrawColor(ren, cr, cg, cb, ca * a);
        ly++;
        draw_part();
        ly--;
    }

    SDL_SetRenderDrawColor(ren, cr, cg, cb, ca);
}

void draw_gradient(context& ctx, unit x, unit y, unit w, unit h,
                   std::uint8_t r1, std::uint8_t g1, std::uint8_t b1,
                   std::uint8_t a1, std::uint8_t r2, std::uint8_t g2,
                   std::uint8_t b2, std::uint8_t a2, bool vertical) noexcept
{
    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(
        0, vertical ? 1 : w, vertical ? h : 1, 32, SDL_PIXELFORMAT_RGBA32);
    SDL_Rect rect{x, y, w, h};
    SDL_LockSurface(surface);

    const auto c = vertical ? h : w;
    auto* pixel = reinterpret_cast<std::uint8_t*>(surface->pixels);
    for (int i = 0; i < c; ++i)
    {
        const float factor = 1.0f * i / c;
        *(pixel++) =
            static_cast<std::uint8_t>((1.0 - factor) * r1 + factor * r2);
        *(pixel++) =
            static_cast<std::uint8_t>((1.0 - factor) * g1 + factor * g2);
        *(pixel++) =
            static_cast<std::uint8_t>((1.0 - factor) * b1 + factor * b2);
        *(pixel++) =
            static_cast<std::uint8_t>((1.0 - factor) * a1 + factor * a2);
    }
    SDL_UnlockSurface(surface);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(ctx.renderer, surface);
    SDL_RenderCopy(renderer(ctx), texture, nullptr, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}

void draw_texture(context& dst, context& src) noexcept
{
    SDL_SetTextureBlendMode(src.texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer(dst), src.texture, nullptr, nullptr);
}

void draw_texture(context& dst, context& src, unit dstx, unit dsty, unit dstw,
                  unit dsth) noexcept
{
    SDL_Rect r{dstx, dsty, dstw, dsth};
    SDL_SetTextureBlendMode(src.texture, SDL_BLENDMODE_BLEND);
    SDL_RenderCopy(renderer(dst), src.texture, nullptr, &r);
}

void apply_alpha_mask(context& dst, context& src) noexcept
{
    assert(dst.texture && src.texture);
    // color from destination and alpha from source
    static auto blend = SDL_ComposeCustomBlendMode(
        SDL_BLENDFACTOR_ZERO, SDL_BLENDFACTOR_ONE, SDL_BLENDOPERATION_ADD,
        SDL_BLENDFACTOR_ONE, SDL_BLENDFACTOR_ZERO, SDL_BLENDOPERATION_ADD);

    SDL_SetTextureBlendMode(src.texture, blend);
    SDL_RenderCopy(renderer(dst), src.texture, nullptr, nullptr);
    SDL_SetTextureBlendMode(src.texture, SDL_BLENDMODE_BLEND);
}

void draw_text_line(context& ctx, const font_context& font, const char* text,
                    unit x, unit y, unit w, unit h) noexcept
{
    if (w == 0)
        w = 0xffff;
    if (h == 0)
        h = 0xffff;
    std::uint8_t cr, cg, cb, ca;
    SDL_GetRenderDrawColor(ctx.renderer, &cr, &cg, &cb, &ca);
    SDL_Surface* s = TTF_RenderUTF8_Blended(font.font, text, {cr, cg, cb});
    if (!s)
        return;
    SDL_Texture* t = SDL_CreateTextureFromSurface(renderer(ctx), s);
    SDL_SetTextureAlphaMod(t, ca);
    SDL_Rect dst_rect{x, y, std::min(s->w, w), std::min(s->h, h)};
    SDL_Rect src_rect{0, 0, std::min(s->w, w), std::min(s->h, h)};
    SDL_RenderCopy(ctx.renderer, t, &src_rect, &dst_rect);
    SDL_FreeSurface(s);
    SDL_DestroyTexture(t);
}

event wait_for_event()
{
    SDL_Event ev;

    for(;;) {
        SDL_WaitEvent(&ev);

        if (ev.type == SDL_WINDOWEVENT)
        {
            switch (ev.window.event)
            {
            case SDL_WINDOWEVENT_RESIZED:
            case SDL_WINDOWEVENT_SIZE_CHANGED:
                return resize_event{ev.window.windowID, ev.window.data1,
                                    ev.window.data2};
            case SDL_WINDOWEVENT_SHOWN:
            case SDL_WINDOWEVENT_HIDDEN:
            case SDL_WINDOWEVENT_EXPOSED:
            case SDL_WINDOWEVENT_MOVED:
            case SDL_WINDOWEVENT_MINIMIZED:
            case SDL_WINDOWEVENT_MAXIMIZED:
            case SDL_WINDOWEVENT_RESTORED:
            case SDL_WINDOWEVENT_ENTER:
            case SDL_WINDOWEVENT_LEAVE:
            case SDL_WINDOWEVENT_FOCUS_GAINED:
            case SDL_WINDOWEVENT_FOCUS_LOST:
            case SDL_WINDOWEVENT_CLOSE:
            case SDL_WINDOWEVENT_TAKE_FOCUS:
            case SDL_WINDOWEVENT_HIT_TEST:
                break;
            }
        } else if (ev.type == SDL_QUIT)
        {
            return quit_event{};
        }
    }
}

template<class... Ts> struct overload : Ts... { using Ts::operator()...; };
template<class... Ts> overload(Ts...) -> overload<Ts...>;

void post_event(event event)
{
    SDL_Event ev
        = std::visit(
            overload{[](resize_event) { return SDL_Event{SDL_QUIT}; },
                     [](auto v) {
                         assert(false); /*posting other event types not supported*/
                         return SDL_Event{};
                     }},
            event);
    const auto res = SDL_PushEvent(&ev);
    assert(res);
}

} // namespace circle::engine
