#include <circle/row.hpp>

namespace circle {

void row::draw(sdl::context ctx) {}

void row::add(item_ptr child)
{
    class anchors& anch = child->anchors;

    item* before{};
    if (!children().empty())
        before = children().back().get();

    item::add(std::move(child));
    if (before)
    {
        anch.set_left(before, anchors::right);
        anch.left_margin = BIND(spacing, spacing);
    }
    else
    {
        anch.set_left(this, anchors::left);
        anch.left_margin = BIND(left_padding, left_padding);
    }
    anch.set_top(this, anchors::top);
    anch.top_margin = BIND(top_padding, top_padding);
    anch.bottom_margin = BIND(bottom_padding, bottom_padding);
    anch.right_margin = BIND(right_padding, right_padding);
}

} // namespace circle