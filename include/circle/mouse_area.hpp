#pragma once

#include <circle/item.hpp>

namespace circle {
class mouse_area : public item
{
public: /*properties*/
    property<bool> contains_mouse;
    property<bool> pressed;
    property<bool> contains_press;

protected:
    void handle_event(const sdl::event& ev) override;
};

} // namespace circle