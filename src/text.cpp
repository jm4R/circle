#include <circle/text.hpp>

#include <circle/application.hpp>

#include <private/circle_sdl.hpp>

namespace circle {

namespace {
template <typename F>
void for_each_line(const std::string& str, F&& f)
{
    auto b = str.begin();
    while (b != str.end())
    {
        auto e = std::find(b, str.end(), '\0');
        f(&*b);
        if (e == str.end())
            return;
        b = ++e;
    }
}
} // namespace

text::text()
{
    font.value_changed.connect([this] { recalculate_size(); });
    value.value_changed().connect([this] { recalculate_size(); });
    color.value_changed().connect([this] { dirty_ = true; });
    width = BIND_EQ(text_width_);
    height = BIND_EQ(text_height_);
}

void text::draw(sdl::context ctx)
{
    render_text(ctx);

    if (ctx_.texture)
    {
        sdl::draw_texture(ctx, ctx_, render_x(), render_y(), width, height);
    }
}

void text::recalculate_size()
{
    sdl::font_context f = app->fonts().get(font);

    unit w{};
    int lines{};
    for_each_line(value, [&](const char* l) {
        const unit cw = sdl::font_get_text_width(f, l);
        w = w > cw ? w : cw;
        ++lines;
    });
    text_width_ = w;
    text_height_ = lines ? (lines - 1) * sdl::font_get_line_skip(f) +
                               lines * sdl::font_get_line_height(f)
                         : 0;
    dirty_ = true;
}

void text::render_text(sdl::context ctx)
{
    if (!dirty_)
        return;

    sdl::texture_destroy(ctx_);
    sdl::texture_init(ctx_, ctx, text_width_, text_height_);
    sdl::font_context f = app->fonts().get(font);
    circle::color c = color;
    sdl::set_color(ctx_, c.r, c.g, c.b, c.a);
    auto lh = sdl::font_get_line_skip(f);
    unit space = 0;
    for_each_line(value, [&](const char* l) {
        sdl::draw_text_line(ctx_, f, l, 0, space, width, height);
        space += lh;
    });

    dirty_ = false;
}

} // namespace circle