#include <circle/object.hpp>

namespace circle {

object::object(object&& other)
{
    enable_tracking_ptr::call_moved();
}

object::~object()
{
    enable_tracking_ptr::call_before_destroyed();
}

object& object::operator=(object&& other)
{
    enable_tracking_ptr::call_moved();
    return *this;
}

bool object::drawable() const
{
    return false;
}

void object::on_event(const event& ev)
{
    // empty by defult
}

}