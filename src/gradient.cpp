#include <circle/gradient.hpp>

#include <algorithm>

namespace circle {

gradient::gradient(enum orientation o,
                   std::initializer_list<gradient_stop> stops)
    : orientation_{o}, stops_{std::move(stops)}
{
}

void gradient::add(gradient_stop stop)
{
    stops_.insert(std::move(stop));
}

const gradient::stops_t& gradient::stops() const
{
    return stops_;
}

void gradient::set_orientation(enum orientation val)
{
    orientation_ = val;
}

enum gradient::orientation gradient::orientation() const
{
    return orientation_;
}

} // namespace circle
