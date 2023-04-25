#pragma once

#include <circle/anchors.hpp>
#include <circle/object.hpp>
#include <circle/reactive/property.hpp>
#include <circle/reactive/tracking_ptr.hpp>
#include <circle/reactive/bind.hpp>

namespace circle {

class item;
using item_ptr = circle::tracking_ptr<item>;

class item : public object, public enable_tracking_ptr<item>
{
public:
public: /*properties*/
    item_ptr parent;
    property<unit> x;
    property<unit> y;
    property<unit> width;
    property<unit> height;
    property<bool> visible;
    circle::anchors anchors;

    item() : anchors{this} {}
    ~item() { enable_tracking_ptr::call_before_destroyed(); }

    item(item&& other)
        : object{std::move(other)}, anchors{std::move(other.anchors)}
    {
        enable_tracking_ptr::call_moved();
    }

    item& operator=(item&& other)
    {
        object::operator=(std::move(other));
        enable_tracking_ptr::call_moved();
        return *this;
    }

    // object interface
public:
    bool drawable() const override;
};

using item_ptr = tracking_ptr<item>;
} // namespace circle