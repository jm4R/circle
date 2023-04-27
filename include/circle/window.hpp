#pragma once

#include <circle/item.hpp>

#include <circle/utils/sdl_utils.hpp>

namespace circle {

class window
{
public:
    window(unsigned w = 600, unsigned h = 800);
    ~window();

    window& operator=(const window&) = delete;

    item& content_item();

    void show();
    void update();

protected:
    void handle_event(const sdl::event& event);
    void redraw();

private:
    void on_resized(int w, int h);

private:
    sdl::window_context ctx_;
    item content_item_;
    unsigned w_{};  // TODO: property
    unsigned h_{};

    friend class ::circle::application;
};

} // namespace circle
