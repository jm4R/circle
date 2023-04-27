#pragma once

#include <circle/color.hpp>

#include <set>

namespace circle {

struct gradient_stop
{
    float position{};
    circle::color color{};

    friend bool operator<(const gradient_stop& left, const gradient_stop& right)
    {
        return left.position < right.position;
    }
};

class gradient
{
public:
    enum orientation
    {
        none,
        vertical,
        horizontal
    };
    using stops_t = std::set<gradient_stop>;

    gradient() = default;
    gradient(orientation o, std::initializer_list<gradient_stop> stops);

    void add(gradient_stop stop);
    const stops_t& stops() const;

    void set_orientation(orientation val);
    orientation orientation() const;

private:
    enum orientation orientation_;
    stops_t stops_;
};

} // namespace circle
