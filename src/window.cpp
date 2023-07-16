#include <circle/window.hpp>

#include <circle/application.hpp>
#include <circle/reactive/bind.hpp>

#include "private/circle_sdl.hpp"

#include <algorithm>
#include <cassert>

namespace circle {

window::window(unsigned w, unsigned h) : width{unit(w)}, height{unit(h)}
{
    assert(app);

    sdl::window_init(ctx_, width, height);
    sdl::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    sdl::clear(ctx_);
    content_item.width = BIND_EQ(width);
    content_item.height = BIND_EQ(height);

    app->windows_.push_back(this);
}

window::~window()
{
    app->windows_.erase(
        std::remove(app->windows_.begin(), app->windows_.end(), this),
        app->windows_.end());
    sdl::window_destroy(ctx_);
}

void window::show()
{
    sdl::window_show(ctx_, true);
    update();
}

void window::update()
{
    redraw(); // TODO: do not redraw directly, mark dirty and limit fps
    sdl::flush(ctx_);
}

void window::handle_event(const event& ev)
{
    if (ev.type == event_type::timer)
        update(); // FIXME: mark dirty

    if (ev.type != event_type::sdl)
        return;

    switch (ev.sdl->type)
    {
    case SDL_WINDOWEVENT:
        if (ctx_.id != ev.sdl->window.windowID)
            return;

        switch (ev.sdl->window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            on_resized(ev.sdl->window.data1, ev.sdl->window.data2);
            break;
        case SDL_WINDOWEVENT_SHOWN:
        case SDL_WINDOWEVENT_HIDDEN:
            break;
        case SDL_WINDOWEVENT_EXPOSED:
            update();
            break;
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
        break;
    case SDL_MOUSEMOTION:
        if (ctx_.id != ev.sdl->motion.windowID)
            return;
        update(); // FIXME: mark dirty
        break;
    case SDL_MOUSEBUTTONDOWN:
    case SDL_MOUSEBUTTONUP:
        if (ctx_.id != ev.sdl->button.windowID)
            return;
        update(); // FIXME: mark dirty
        break;
    }
}

void window::redraw()
{
    sdl::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    sdl::clear(ctx_);
    content_item.render(ctx_);
}

void window::on_resized(int w, int h)
{
    width = w;
    height = h;
    update();
}

} // namespace circle
