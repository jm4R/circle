#include <circle/column.hpp>

#include <private/circle_sdl.hpp>

namespace circle {

void column::draw(sdl::context ctx) {}

void column::add(item_ptr child)
{
    class anchors& anch = child->anchors;

    item* before{};
    if (!children().empty())
        before = children().back().get();

    item::add(std::move(child));
    if (before)
    {
        anch.set_top(before, anchors::bottom);
        anch.top_margin = BIND(spacing, spacing);
    }
    else
    {
        anch.set_top(this, anchors::top);
        anch.top_margin = BIND(top_padding, top_padding);
    }
    anch.set_left(this, anchors::left);
    anch.left_margin = BIND(left_padding, left_padding);
    anch.right_margin = BIND(right_padding, right_padding);
    anch.bottom_margin = BIND(bottom_padding, bottom_padding);
}

} // namespace circle