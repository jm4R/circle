#include <circle/object.hpp>

namespace circle {

bool object::drawable() const
{
    return false;
}

void object::on_event(const event& ev)
{
    // empty by defult
}

}