#include <circle/mouse_area.hpp>

#include <private/circle_sdl.hpp>

namespace circle {

} // namespace circle

void circle::mouse_area::handle_event(const sdl::event& ev)
{
    Sint32 mx, my;
    if (ev.type == SDL_MOUSEMOTION)
    {
        const auto rx = render_x();
        const auto ry = render_y();
        contains_mouse =
            (ev.motion.x >= rx && ev.motion.y >= ry &&
             ev.motion.x < rx + width && ev.motion.y < ry + height);
    }
    else if (ev.type == SDL_MOUSEBUTTONDOWN || ev.type == SDL_MOUSEBUTTONUP)
    {
        pressed = ev.button.state == SDL_PRESSED;
    }
    contains_press = contains_mouse && pressed;
}