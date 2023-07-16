#include <circle/mouse_area.hpp>

#include <circle/application.hpp>
#include <private/circle_sdl.hpp>

namespace circle {

mouse_area::mouse_area()
{
    assert(circle::app);
    circle::app->register_event_handler(this);
}

void mouse_area::on_event(const event& ev)
{
    if (ev.type != event_type::sdl)
        return;

    auto& sev = ev.sdl;
    Sint32 mx, my;
    if (ev.sdl->type == SDL_MOUSEMOTION)
    {
        const auto rx = render_x();
        const auto ry = render_y();
        contains_mouse =
            (ev.sdl->motion.x >= rx && ev.sdl->motion.y >= ry &&
             ev.sdl->motion.x < rx + width && ev.sdl->motion.y < ry + height);
    }
    else if (ev.sdl->type == SDL_MOUSEBUTTONDOWN ||
             ev.sdl->type == SDL_MOUSEBUTTONUP)
    {
        pressed = ev.sdl->button.state == SDL_PRESSED;
        if (!pressed)
            clicked();
    }
    contains_press = contains_mouse && pressed;
}

} // namespace circle