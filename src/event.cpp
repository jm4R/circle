#include <circle/event.hpp>
#include <private/circle_sdl.hpp>

namespace circle {

constexpr static Uint32 INVALID_EVENT = 0xffffffff;
static Uint32 SDL_CIRCLE_EVENT = INVALID_EVENT;

bool detail::register_circle_events()
{
    SDL_CIRCLE_EVENT = SDL_RegisterEvents(1);
    return SDL_CIRCLE_EVENT != INVALID_EVENT;
}

event to_circle_event(const sdl::event& ev) noexcept
{
    if (ev.type == SDL_CIRCLE_EVENT)
    {
        event res{};
        res.type = static_cast<event_type>(ev.user.code);
        switch (res.type)
        {
        case event_type::sdl:
            res.sdl = &ev;
            break;
        case event_type::timer:
            res.timer =
                timer_event{static_cast<sdl::timer_id>(ev.user.windowID)};
            break;
        case event_type::user:
            res.user_data = user_event{ev.user.data1, ev.user.data2};
            break;
        }
        return res;
    }

    return event{event_type::sdl, &ev};
}

sdl::event to_sdl_event(const event& ev) noexcept
{
    sdl::event res{};
    res.type = SDL_CIRCLE_EVENT;
    res.user = SDL_UserEvent{
        SDL_CIRCLE_EVENT, 0, 0, static_cast<Sint32>(ev.type), nullptr, nullptr};
    switch (ev.type)
    {
    case event_type::sdl:
        break;
    case event_type::timer:
        static_assert(sizeof(res.user.windowID) == sizeof(Uint32));
        res.user.windowID = static_cast<Uint32>(ev.timer.id);
        break;
    case event_type::user:
        res.user.data1 = ev.user_data.data1;
        res.user.data2 = ev.user_data.data2;
        break;
    }

    return res;
}

} // namespace circle