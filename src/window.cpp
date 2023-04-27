#include <circle/window.hpp>

#include <circle/application.hpp>

#include "private/circle_sdl.hpp"

#include <algorithm>
#include <cassert>

namespace circle {

window::window(unsigned w, unsigned h) : w_{w}, h_{h}
{
    assert(app);

    sdl::window_init(ctx_, w_, h_);
    sdl::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    sdl::clear(ctx_);
    // content_item_.set_ctx(&ctx_);

    content_item().width = w;
    content_item().height = h;

    app->windows_.push_back(this);
}

window::~window()
{
    app->windows_.erase(
        std::remove(app->windows_.begin(), app->windows_.end(), this),
        app->windows_.end());
    sdl::window_destroy(ctx_);
}

item& window::content_item()
{
    return content_item_;
}

void window::show()
{
    sdl::window_show(ctx_, true);
    update();
}

void window::update()
{
    redraw();
    sdl::flush(ctx_);
}

void window::handle_event(const sdl::event& ev)
{
    if (ev.type == SDL_WINDOWEVENT)
    {
        if (ctx_.id != ev.window.windowID)
            return;

        switch (ev.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
        case SDL_WINDOWEVENT_SIZE_CHANGED:
            on_resized(ev.window.data1, ev.window.data2);
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
}

void window::redraw()
{
    sdl::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    sdl::clear(ctx_);
    // content_item().render();
}

void window::on_resized(int w, int h)
{
    w_ = w;
    h_ = h;
    content_item().width = w_;
    content_item().height = h_;
    update();
}

} // namespace circle
