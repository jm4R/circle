#pragma once

#include <circle/item.hpp>

namespace circle {
class mouse_area : public item
{
public: /*properties*/
    property<bool> contains_mouse;
    property<bool> pressed;
    property<bool> contains_press;

    signal<> clicked;

public:
    explicit mouse_area();

protected:
    void on_event(const event& ev) override;
};

} // namespace circle