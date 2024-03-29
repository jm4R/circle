#pragma once

#include <circle/fwd.hpp>
#include <circle/reactive/property.hpp>

namespace circle {

class item;
using item_ptr = circle::tracking_ptr<item>;

class anchors
{
public: /*properties*/
    property<unit> left_margin;
    property<unit> right_margin;
    property<unit> top_margin;
    property<unit> bottom_margin;
    property<unit> margins;

public:
    enum horizontal_mode
    {
        left,
        horizontal_center,
        right
    };

    enum vertical_mode
    {
        top,
        vertical_center,
        bottom
    };

    anchors(item_ptr parent);

    signal<>& horizontal_changed() { return horizontal_changed_; }
    signal<>& vertical_changed() { return vertical_changed_; }

    // horizontal
    void set(horizontal_mode destination_anchor, item_ptr source_item,
             horizontal_mode source_anchor);
    void set_left(item_ptr source_item, horizontal_mode source_anchor);
    void set_left(item& source_item, horizontal_mode source_anchor)
    {
        set_left(&source_item, source_anchor);
    }
    void set_horizontal_center(item_ptr source_item,
                               horizontal_mode source_anchor);
    void set_horizontal_center(item& source_item, horizontal_mode source_anchor)
    {
        set_horizontal_center(&source_item, source_anchor);
    }
    void set_right(item_ptr source_item, horizontal_mode source_anchor);
    void set_right(item& source_item, horizontal_mode source_anchor)
    {
        set_right(&source_item, source_anchor);
    }

    // vertical
    void set(vertical_mode destination_anchor, item_ptr source_item,
             vertical_mode source_anchor);
    void set_top(item_ptr source_item, vertical_mode source_anchor);
    void set_top(item& source_item, vertical_mode source_anchor)
    {
        set_top(&source_item, source_anchor);
    }
    void set_vertical_center(item_ptr source_item, vertical_mode source_anchor);
    void set_vertical_center(item& source_item, vertical_mode source_anchor)
    {
        set_vertical_center(&source_item, source_anchor);
    }
    void set_bottom(item_ptr source_item, vertical_mode source_anchor);
    void set_bottom(item& source_item, vertical_mode source_anchor)
    {
        set_bottom(&source_item, source_anchor);
    }

    // convenience anchors
    void fill(item_ptr source_item);
    void fill(item& source_item) { fill(&source_item); }
    void center_in(item_ptr source_item);
    void center_in(item& source_item) { center_in(&source_item); }

    void clear();

private:
    void recalculate_horizontal();
    void recalculate_vertical();

private:
    struct horizontal_entry
    {
        item_ptr item;
        horizontal_mode mode;
    };

    struct vertical_entry
    {
        item_ptr item;
        vertical_mode mode;
    };

    void assert_precondition(item_ptr& item) const;
    unit calc_x(horizontal_entry entry) const;
    unit calc_y(vertical_entry entry) const;
    bool overrides_x() const;
    bool overrides_y() const;
    bool overrides_width() const;
    bool overrides_height() const;

    // values with margins included, relative to parent
    unit calc_x() const;
    unit calc_y() const;
    unit calc_width() const;
    unit calc_height() const;

private:
    item_ptr item_;

    horizontal_entry left_;
    horizontal_entry horizontal_center_;
    horizontal_entry right_;

    vertical_entry top_;
    vertical_entry vertical_center_;
    vertical_entry bottom_;

    signal<> horizontal_changed_;
    signal<> vertical_changed_;
};

} // namespace circle