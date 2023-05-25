#include <circle/column.hpp>

namespace circle {

void column::draw(sdl::context ctx) {}

column::column()
{
    padding.value_changed().connect([this](unit val){
        left_padding = val;
        right_padding = val;
        top_padding = val;
        bottom_padding = val;
    });
}

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
        anch.top_margin = BIND_EQ(spacing);
    }
    else
    {
        anch.set_top(this, anchors::top);
        anch.top_margin = BIND_EQ(top_padding);
    }
    anch.set_left(this, anchors::left);
    anch.left_margin = BIND_EQ(left_padding);
    anch.right_margin = BIND_EQ(right_padding);
    anch.bottom_margin = BIND_EQ(bottom_padding);
}

} // namespace circle