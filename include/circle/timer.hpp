#pragma once

#include <circle/object.hpp>
#include <circle/utils/sdl_utils.hpp>

#include <circle/reactive/signal.hpp>

namespace circle {

class timer : public object
{
public: /*properties*/
    property<unsigned> interval;
    property<bool> repeat;
    property<bool> running;
    property<bool> triggered_on_start;

    signal<> triggered;

public:
    explicit timer();
    ~timer();

    void start();
    void stop();

protected:
    void on_event(const event& ev) override;

private:
    void post_triggered();

private:
    sdl::timer_id id_{};
};
} // namespace circle