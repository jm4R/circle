#pragma once

#include <circle/item.hpp>
#include <circle/reactive/property.hpp>
#include <circle/utils/sdl_utils.hpp>

namespace circle {

class window
{
public: /*properties*/
    property<unit> width;
    property<unit> height;
    item content_item;

public:
    window(unsigned w = 600, unsigned h = 800);
    ~window();

    window& operator=(const window&) = delete;

    void show();
    void update();

protected:
    void handle_event(const sdl::event& event);
    void redraw();

private:
    void on_resized(int w, int h);

private:
    sdl::window_context ctx_;

    friend class ::circle::application;
};

} // namespace circle
