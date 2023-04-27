#pragma once

#include <cstdint>

namespace circle
{

class color
{
public:
    explicit color() = default;
    color(std::uint8_t r, std::uint8_t g, std::uint8_t b, std::uint8_t a = 0xff) noexcept;
    color(std::uint32_t argb) noexcept;

    bool operator==(color other) const { return r==other.r && g==other.g && b == other.b && a == other.a; }

    std::uint8_t r{0xff};
    std::uint8_t g{0xff};
    std::uint8_t b{0xff};
    std::uint8_t a{0xff};

};

inline static color black = 0x000000;
inline static color white = 0xffffff;
inline static color red = 0xff0000;
inline static color green = 0x00ff00;
inline static color blue = 0x0000ff;
inline static color cyan = 0x00ffff;
inline static color magenta = 0xff00ff;
inline static color yellow = 0xffff00;

}
