#pragma once

#include <circle/anchors.hpp>
#include <circle/object.hpp>
#include <circle/reactive/bind.hpp>
#include <circle/reactive/property.hpp>
#include <circle/reactive/tracking_ptr.hpp>
#include <circle/utils/sdl_utils.hpp>

#include <type_traits>

namespace circle {

class item;
using item_ptr = circle::tracking_ptr<item>;

class item : public object, public enable_tracking_ptr<item>
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
    ~item();

    item(item&& other);
    item& operator=(item&& other);

    using enable_tracking_ptr<item>::moved;
    using enable_tracking_ptr<item>::before_destroyed;

    virtual void add(item_ptr child);

    template <typename T>
    T& add()
    {
        auto ptr = std::make_unique<T>();
        auto raw_ptr = ptr.get();
        if constexpr (std::is_constructible_v<item_ptr, T*>)
        {
            add(item_ptr{raw_ptr});
        }
        owning_objects_.push_back(std::move(ptr));
        return *raw_ptr;
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
    std::vector<std::unique_ptr<object>> owning_objects_;
    std::vector<item_ptr> children_;
};
} // namespace circle