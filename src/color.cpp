#include <circle/color.hpp>

namespace circle {

color::color(std::uint8_t r, std::uint8_t g, std::uint8_t b,
             std::uint8_t a) noexcept
    : r{r}, g{g}, b{b}, a{a}
{
}

color::color(uint32_t rgb) noexcept
    : color((rgb >> 16) & 0xff, (rgb >> 8) & 0xff, (rgb >> 0) & 0xff)
{
}

} // namespace circle
