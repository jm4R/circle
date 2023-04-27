#pragma once

#include <circle/color.hpp>
#include <circle/gradient.hpp>
#include <circle/item.hpp>

namespace circle {
class rectangle : public item
{
public: /*properties*/
    property<circle::color> color;
    property<unit> radius;
    property<bool> antialiasing{true};
    property<circle::gradient> gradient;

protected:
    void draw(sdl::context ctx) override;

private:
    void draw_simple(sdl::context& ctx, class color c);
    void draw_gradient_vertical(sdl::context& ctx, unit x, unit y);
    void draw_gradient_horizontal(sdl::context& ctx, unit x, unit y);
    void draw_gradient_with_radius(sdl::context& ctx);
};

} // namespace circle