#pragma once

#include <circle/color.hpp>
#include <circle/gradient.hpp>
#include <circle/item.hpp>

namespace circle {
class row : public item
{
public: /*properties*/
    property<unit> spacing;
    property<unit> top_padding;
    property<unit> left_padding;
    property<unit> right_padding;
    property<unit> bottom_padding;
    property<unit> padding;

public:
    explicit row();

    using item::add;
    void add(item_ptr child) override;

protected:
    void draw(sdl::context ctx) override;
};

} // namespace circle