#include <circle/anchors.hpp>

#include <circle/item.hpp>

namespace circle {

#define CIRCLE_UB(...) // TODO

namespace {
template <typename T>
bool isset(T& anchor)
{
    return !!anchor.item;
}
} // namespace

anchors::anchors(item_ptr parent) : item_{std::move(parent)}
{
    // TODO: don't capture this
    margins.value_changed().connect([this](unit v) {
        left_margin = v;
        right_margin = v;
        top_margin = v;
        bottom_margin = v;
    });
    left_margin.value_changed().connect([this] { recalculate_horizontal(); });
    right_margin.value_changed().connect([this] { recalculate_horizontal(); });
    item_->x.value_changed().connect([this] {
        if (overrides_x())
        {
            recalculate_horizontal();
        }
        else
        {
            horizontal_changed_();
        }
    });
    item_->width.value_changed().connect([this] {
        if (overrides_width())
        {
            recalculate_horizontal();
        }
        else
        {
            horizontal_changed_();
        }
    });

    top_margin.value_changed().connect([this] { recalculate_vertical(); });
    bottom_margin.value_changed().connect([this] { recalculate_vertical(); });
    item_->y.value_changed().connect([this] {
        if (overrides_y())
        {
            recalculate_vertical();
        }
        else
        {
            vertical_changed_();
        }
    });
    item_->height.value_changed().connect([this] {
        if (overrides_height())
        {
            recalculate_vertical();
        }
        else
        {
            vertical_changed_();
        }
    });
}

void anchors::set(horizontal_mode destination_anchor, item_ptr source_item,
                  horizontal_mode source_anchor)
{
    switch (destination_anchor)
    {
    case left:
        set_left(std::move(source_item), source_anchor);
        break;
    case horizontal_center:
        set_horizontal_center(std::move(source_item), source_anchor);
        break;
    case right:
        set_right(std::move(source_item), source_anchor);
        break;
    }
}

void anchors::set_left(item_ptr source_item, horizontal_mode source_anchor)
{
    CIRCLE_UB(!horizontal_center_ || !right_,
              "Cannot specify left, right, and horizontal_center anchors at "
              "the same time");
    left_.item = std::move(source_item);
    left_.mode = source_anchor;
    left_.item->anchors.horizontal_changed().connect(
        [this] { recalculate_horizontal(); });

    recalculate_horizontal();
}

void anchors::set_horizontal_center(item_ptr source_item,
                                    horizontal_mode source_anchor)
{
    CIRCLE_UB(!isset(left_) || !isset(right_),
              "Cannot specify left, right, and horizontal_center anchors at "
              "the same time");
    horizontal_center_.item = std::move(source_item);
    horizontal_center_.mode = source_anchor;
    horizontal_center_.item->anchors.horizontal_changed().connect(
        [this] { recalculate_horizontal(); });

    recalculate_horizontal();
}

void anchors::set_right(item_ptr source_item, horizontal_mode source_anchor)
{
    CIRCLE_UB(!isset(left_) || !isset(horizontal_center_),
              "Cannot specify left, right, and horizontal_center anchors at "
              "the same time");
    right_.item = std::move(source_item);
    right_.mode = source_anchor;
    right_.item->anchors.horizontal_changed().connect(
        [this] { recalculate_horizontal(); });

    recalculate_horizontal();
}

void anchors::set(vertical_mode destination_anchor, item_ptr source_item,
                  vertical_mode source_anchor)
{
    switch (destination_anchor)
    {
    case top:
        set_top(std::move(source_item), source_anchor);
        break;
    case vertical_center:
        set_vertical_center(std::move(source_item), source_anchor);
        break;
    case bottom:
        set_bottom(std::move(source_item), source_anchor);
        break;
    }
}

void anchors::set_top(item_ptr source_item, vertical_mode source_anchor)
{
    CIRCLE_UB(!isset(bottom_) || !isset(vertical_center_),
              "Cannot specify top, bottom, and vertical_center anchors at the "
              "same time");
    top_.item = std::move(source_item);
    top_.mode = source_anchor;
    top_.item->anchors.vertical_changed().connect(
        [this] { recalculate_vertical(); });

    recalculate_vertical();
}

void anchors::set_vertical_center(item_ptr source_item,
                                  vertical_mode source_anchor)
{
    CIRCLE_UB(!isset(top_) || !isset(bottom_),
              "Cannot specify top, bottom, and vertical_center anchors at "
              "the same time");
    vertical_center_.item = std::move(source_item);
    vertical_center_.mode = source_anchor;
    vertical_center_.item->anchors.vertical_changed().connect(
        [this] { recalculate_vertical(); });

    recalculate_vertical();
}

void anchors::set_bottom(item_ptr source_item, vertical_mode source_anchor)
{
    CIRCLE_UB(!isset(top_) || !isset(vertical_center_),
              "Cannot specify top, bottom, and vertical_center anchors at "
              "the same time");
    bottom_.item = std::move(source_item);
    bottom_.mode = source_anchor;
    bottom_.item->anchors.vertical_changed().connect(
        [this] { recalculate_vertical(); });

    recalculate_vertical();
}

void anchors::fill(item_ptr source_item)
{
    clear();
    set_top(source_item, anchors::top);
    set_bottom(source_item, anchors::bottom);
    set_left(source_item, anchors::left);
    set_right(source_item, anchors::right);
}

void anchors::center_in(item_ptr source_item)
{
    clear();
    set_horizontal_center(source_item, anchors::horizontal_center);
    set_vertical_center(source_item, anchors::vertical_center);
}

void anchors::clear()
{
    left_ = {};
    horizontal_center_ = {};
    right_ = {};

    top_ = {};
    vertical_center_ = {};
    bottom_ = {};
}

void anchors::recalculate_horizontal()
{
    bool changed = false;

    if (overrides_x() && item_->x.assign(calc_x()))
    {
        changed = true;
    }

    if (overrides_width() && item_->width.assign(calc_width()))
    {
        changed = true;
    }

    if (changed)
    {
        horizontal_changed_();
    }
}

void anchors::recalculate_vertical()
{
    bool changed = false;

    if (overrides_y() && item_->y.assign(calc_y()))
    {
        changed = true;
    }

    if (overrides_height() && item_->height.assign(calc_height()))
    {
        changed = true;
    }

    if (changed)
    {
        vertical_changed_();
    }
}

void anchors::assert_precondition(item_ptr& it) const
{
    // For performance reasons, you can only anchor an item to its siblings and
    // direct parent.
    assert(!!it && it != item_ && item_->parent == it ||
           item_->parent == it->parent);
}

unit anchors::calc_x(horizontal_entry entry) const
{
    assert_precondition(entry.item);
    const bool siblings = entry.item->parent == item_->parent;
    const auto x = siblings ? *entry.item->x : 0;
    switch (entry.mode)
    {
    case anchors::left:
        return x;
    case anchors::horizontal_center:
        return x + entry.item->width / 2;
    case anchors::right:
        return x + entry.item->width;
    }

    return {};
}

unit anchors::calc_y(vertical_entry entry) const
{
    assert_precondition(entry.item);
    const bool siblings = entry.item->parent == item_->parent;
    const auto y = siblings ? *entry.item->y : 0;
    switch (entry.mode)
    {
    case anchors::top:
        return y;
    case anchors::vertical_center:
        return y + entry.item->height / 2;
    case anchors::bottom:
        return y + entry.item->height;
    }

    return {};
}

bool anchors::overrides_x() const
{
    return isset(left_) || isset(horizontal_center_) || isset(right_);
}

bool anchors::overrides_y() const
{
    return isset(top_) || isset(vertical_center_) || isset(bottom_);
}

bool anchors::overrides_width() const
{
    int set_count = isset(left_) + isset(right_) + isset(horizontal_center_);
    return set_count == 2;
}

bool anchors::overrides_height() const
{
    int set_count = isset(top_) + isset(bottom_) + isset(vertical_center_);
    return set_count == 2;
}

unit anchors::calc_x() const
{
    if (isset(left_))
        return left_margin + calc_x(left_);
    else if (isset(right_))
        return -right_margin + calc_x(right_) - item_->width;
    else if (isset(horizontal_center_))
        return calc_x(horizontal_center_) - item_->width / 2;
    else
        return item_->x;
}

unit anchors::calc_y() const
{
    if (isset(top_))
        return top_margin + calc_y(top_);
    else if (isset(bottom_))
        return -bottom_margin + calc_y(bottom_) - item_->height;
    else if (isset(vertical_center_))
        return calc_y(vertical_center_) - item_->height / 2;
    else
        return item_->y;
}

unit anchors::calc_width() const
{
    unit res{};
    if (isset(left_) && isset(right_))
    {
        res = calc_x(right_) - calc_x(left_) - left_margin - right_margin;
    }
    else
    {
        return item_->width;
    }
    return res > 0 ? res : 0;
}

unit anchors::calc_height() const
{
    unit res{};
    if (isset(top_) && isset(bottom_))
    {
        res = calc_y(bottom_) - calc_y(top_) - top_margin - bottom_margin;
    }
    else
    {
        return item_->height;
    }
    return res > 0 ? res : 0;
}

} // namespace circle