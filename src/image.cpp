#include <circle/image.hpp>

#include <private/circle_sdl.hpp>

#include <algorithm>

namespace circle {

image::image()
{
    source_width = BIND_EQ(loaded_width_);
    source_height = BIND_EQ(loaded_height_);
    width = BIND(source_width, loaded_width_,
                 std::min(source_width, loaded_width_));
    height = BIND(source_height, loaded_height_,
                  std::min(source_height, loaded_height_));
    // TODO: don't capture this
    source.value_changed().connect([this] { needs_reload_ = true; });
}

void image::draw(sdl::context ctx)
{
    load(ctx);

    if (ctx_.texture)
    {
        sdl::draw_texture(ctx, ctx_, render_x(), render_y(), width, height);
    }
}

void image::load(sdl::context ctx)
{
    if (!needs_reload_)
        return;

    if (source.get().empty())
        return;

    sdl::image_init(ctx_, ctx, source.get().c_str());
    if (!ctx_.texture)
        return;

    const sdl::size size = sdl::texture_size(ctx_);
    loaded_width_ = size.w;
    loaded_height_ = size.h;

    if (loaded_width_ > source_width || loaded_height_ > source_height)
    {
        // TODO: resize
    }

    needs_reload_ = false;
}

} // namespace circle