#pragma once

#include <circle/anchors.hpp>
#include <circle/object.hpp>
#include <circle/reactive/bind.hpp>
#include <circle/reactive/property.hpp>
#include <circle/reactive/ptr.hpp>
#include <circle/utils/sdl_utils.hpp>

namespace circle {

class item;
using item_ptr = tracking_ptr<item>;

class item : public object
{
public: /*properties*/
    item_ptr parent;
    property<unit> x;
    property<unit> y;
    property<unit> width;
    property<unit> height;
    property<bool> visible{true};
    property<bool> enabled{true};
    circle::anchors anchors;

public:
    item();

    virtual void add(item_ptr child);

    template <typename T>
    tracking_ptr<T> add()
    {
        auto ptr = make_ptr<T>();
        add(ptr);
        owning_items_.emplace_back(std::move(ptr));
        return static_pointer_cast<T>(tracking_ptr{owning_items_.back()});
    }

    std::vector<item_ptr>& children() { return children_; }

    void render(sdl::context& ctx);

protected:
    virtual void draw(sdl::context ctx);

protected:
    unit render_x() const
    {
        if (parent)
        {
            return parent->render_x() + x;
        }
        else
        {
            return x;
        }
    }

    unit render_y() const
    {
        if (parent)
        {
            return parent->render_y() + y;
        }
        else
        {
            return y;
        }
    }

private:
    std::vector<ptr<item>> owning_items_;
    std::vector<item_ptr> children_;
};
} // namespace circle