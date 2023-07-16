#pragma once

#include <circle/utils/sdl_utils.hpp>

#include <cstdint>

namespace circle {

namespace detail {
bool register_circle_events();
}

enum class event_type : std::int32_t
{
    sdl,
    timer,
    user = 1000
};

struct timer_event
{
    sdl::timer_id id;
};

struct user_event
{
    void* data1;
    void* data2;
};

struct event
{
    event_type type;
    union
    {
        const sdl::event* sdl;
        timer_event timer;
        user_event user_data;
    };
};

event to_circle_event(const sdl::event& ev) noexcept;
sdl::event to_sdl_event(const event& ev) noexcept;

} // namespace circle