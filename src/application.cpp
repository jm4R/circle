#include <circle/application.hpp>

#include <circle/window.hpp>

#include "private/circle_sdl.hpp"
#include "private/standard_fonts/standard_fonts.hpp"

#include <cassert>

namespace circle {

application_base::application_base()
{
    static bool once_guard{};

    assert(!once_guard);
    once_guard = true;

    sdl::init_engine();
}

application_base::~application_base()
{
    sdl::destroy_engine();
}

application::application()
{
    fonts_.add_memory(reinterpret_cast<std::byte*>(Roboto_Regular_ttf),
                      Roboto_Regular_ttf_len);
    app = this;
}

application::~application()
{
    app = nullptr;
}

int application::exec()
{
    while (!quit_)
    {
        sdl::event ev;
        sdl::event_wait(ev);
        handle_event(ev);
    }
    return 0;
}

void application::quit()
{
    sdl::event ev{};
    ev.type = SDL_QUIT;
    sdl::event_push(ev);
}

font_database& application::fonts()
{
    return fonts_;
}

void application::handle_event(const sdl::event& ev)
{
    if (ev.type == SDL_QUIT)
    {
        quit_ = true;
    }

    for (auto* w : windows_)
    {
        w->handle_event(ev);
    }
}

} // namespace circle
