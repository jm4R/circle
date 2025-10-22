#pragma once

#include <circle/reactive/property.hpp>
#include <circle/reactive/ptr.hpp>
#include <circle/utils/sdl_utils.hpp>
#include <circle/event.hpp>

namespace circle {

class object : public circle::enable_tracking_from_this<object>
{
public:
    explicit object() = default;

    object(const object&) = delete;
    object& operator=(const object) = delete;
    object(object&&) = delete;
    object& operator=(object&&) = delete;

    virtual bool drawable() const;

protected:
    virtual void on_event(const event& ev);

private:
    friend class application;
};

using object_ptr = circle::ptr<object>;
using object_tracking = circle::tracking_ptr<object>;

} // namespace circle