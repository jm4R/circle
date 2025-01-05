#include <circle/timer.hpp>

#include <circle/application.hpp>
#include <circle/event.hpp>
#include <private/circle_sdl.hpp>

namespace circle {

timer::timer()
{
    assert(circle::app);
    circle::app->register_event_handler(this);
    // TODO: don't capture this
    running.value_changed().connect([this](bool val) {
        if (val)
        {
            start();
        }
        else
        {
            stop();
        }
    });
}

timer::~timer()
{
    stop();
}

void timer::start()
{
    running = true;
    if (triggered_on_start)
        post_triggered();
    id_ = sdl::start_timer(
        interval,
        [](std::uint32_t i, void* p) {
            auto& self = *reinterpret_cast<timer*>(p);
            self.post_triggered();
            return self.running ? std::uint32_t{self.interval.get()}
                                : std::uint32_t{};
        },
        this);
}

void timer::stop()
{
    running = false;
    sdl::stop_timer(id_);
}

void timer::on_event(const event& ev)
{
    if (running && ev.type == event_type::timer && ev.timer.id == id_)
    {
        triggered();
    }
}

void timer::post_triggered()
{
    // warning: must be thread-safe
    event ev{};
    ev.type = event_type::timer;
    ev.timer = timer_event{id_};
    circle::app->post_event(ev);
}

} // namespace circle