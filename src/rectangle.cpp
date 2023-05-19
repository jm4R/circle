#include <circle/rectangle.hpp>

#include <private/circle_sdl.hpp>

namespace circle {

void rectangle::draw(sdl::context ctx)
{
    const auto& g = *gradient;
    if (g.orientation() == gradient::none)
        draw_simple(ctx, color);
    else if (radius > 0)
        draw_gradient_with_radius(ctx);
    else if (g.orientation() == gradient::vertical)
        draw_gradient_vertical(ctx, render_x(), render_y());
    else // if (gradient_->orientation() == gradient::horizontal)
        draw_gradient_horizontal(ctx, render_x(), render_y());
}

void rectangle::draw_simple(sdl::context& ctx, circle::color c)
{
    sdl::set_color(ctx, c.r, c.g, c.b, c.a);
    sdl::draw_rectangle_filled_rounded(ctx, render_x(), render_y(), width,
                                       height, radius, antialiasing);
}

void rectangle::draw_gradient_vertical(sdl::context& ctx, unit x, unit y)
{
    const auto& stops = gradient.get().stops();

    if (stops.size() == 0)
        return draw_simple(ctx, white);
    if (stops.size() == 1)
        return draw_simple(ctx, stops.begin()->color);

    const auto w = *width;
    const auto h = *height;

    auto prev_stop = *stops.begin();
    if (h * prev_stop.position > 0)
    {
        sdl::set_color(ctx, prev_stop.color.r, prev_stop.color.g,
                          prev_stop.color.b, prev_stop.color.a);
        sdl::draw_rectangle_filled(ctx, x, y, w, h * prev_stop.position);
    }

    bool first = true;
    for (const auto& s : stops)
    {
        if (first)
        {
            first = false;
            continue;
        }
        const unit start = h * prev_stop.position;
        const unit stop = h * s.position;
        const auto& c1 = prev_stop.color;
        const auto& c2 = s.color;
        sdl::draw_gradient(ctx, x, y + start, w, stop - start, c1.r, c1.g,
                              c1.b, c1.a, c2.r, c2.g, c2.b, c2.a, true);
        prev_stop = s;
    }

    if (h * prev_stop.position < h)
    {
        sdl::set_color(ctx, prev_stop.color.r, prev_stop.color.g,
                          prev_stop.color.b, prev_stop.color.a);
        sdl::draw_rectangle_filled(ctx, x, y + h * prev_stop.position, w,
                                   h - h * prev_stop.position + 1);
    }
}

void rectangle::draw_gradient_horizontal(sdl::context& ctx, unit x, unit y)
{
    const auto& stops = gradient.get().stops();

    if (stops.size() == 0)
        return draw_simple(ctx, white);
    if (stops.size() == 1)
        return draw_simple(ctx, stops.begin()->color);

    const auto w = *width;
    const auto h = *height;

    auto prev_stop = *stops.begin();
    if (w * prev_stop.position > 0)
    {
        sdl::set_color(ctx, prev_stop.color.r, prev_stop.color.g,
                          prev_stop.color.b, prev_stop.color.a);
        sdl::draw_rectangle_filled(ctx, x, y, w * prev_stop.position, h);
    }

    bool first = true;
    for (const auto& s : stops)
    {
        if (first)
        {
            first = false;
            continue;
        }
        const unit start = w * prev_stop.position;
        const unit stop = w * s.position;
        const auto& c1 = prev_stop.color;
        const auto& c2 = s.color;
        sdl::draw_gradient(ctx, x + start, y, stop - start, h, c1.r, c1.g,
                              c1.b, c1.a, c2.r, c2.g, c2.b, c2.a, false);
        prev_stop = s;
    }

    if (w * prev_stop.position < w)
    {
        sdl::set_color(ctx, prev_stop.color.r, prev_stop.color.g,
                          prev_stop.color.b, prev_stop.color.a);
        sdl::draw_rectangle_filled(ctx, x + w * prev_stop.position, y,
                                   w - w * prev_stop.position + 1, h);
    }
}

void rectangle::draw_gradient_with_radius(sdl::context& ctx)
{
    const auto x = render_x();
    const auto y = render_y();
    const auto w = *width;
    const auto h = *height;
    if (w == 0 || h == 0)
        return;
    sdl::context texture;
    sdl::texture_init(texture, ctx, w, h);

    if (gradient.get().orientation() == gradient::vertical)
        draw_gradient_vertical(texture, 0, 0);
    else // if (gradient_->orientation() == gradient::horizontal)
        draw_gradient_horizontal(texture, 0, 0);

    sdl::context mask;
    sdl::texture_init(mask, ctx, w, h);
    sdl::set_color(mask, 0, 0, 0, 0x00);
    sdl::clear(mask);
    sdl::set_color(mask, 0, 0, 0, 0xff);
    sdl::draw_rectangle_filled_rounded(mask, 0, 0, w, h, radius, antialiasing);

    sdl::apply_alpha_mask(texture, mask);
    sdl::draw_texture(ctx, texture, x, y, w, h);

    sdl::texture_destroy(texture);
    sdl::texture_destroy(mask);
}

} // namespace circle