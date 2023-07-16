#pragma once

#include <circle/reactive/property.hpp>
#include <circle/reactive/tracking_ptr.hpp>
#include <circle/utils/sdl_utils.hpp>
#include <circle/event.hpp>

namespace circle {

class object : public enable_tracking_ptr<object>
{
public:
    object() = default;
    virtual ~object();

    object(object&& other);
    object& operator=(object&& other);

    virtual bool drawable() const;

protected:
    virtual void on_event(const event& ev);

private:
    friend class application;
};

using object_ptr = circle::tracking_ptr<object>;

} // namespace circle