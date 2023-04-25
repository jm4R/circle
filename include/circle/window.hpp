#pragma once

#include <circle/item.hpp>

#include <circle/private/draw_engine.hpp> // FIXME: remove private header

namespace circle {

class window
{
public:
    window(unsigned w = 600, unsigned h = 400);
    ~window();

    window& operator=(const window&) = delete;

    item& content_item();

    void show();
    void update();

protected:
    void redraw();
    void handle_event(const engine::event& event);
    void handle_event(const engine::resize_event& event);

private:
    engine::window_context ctx_;
    item content_item_;
    unsigned w_{};
    unsigned h_{};
};

} // namespace circle
