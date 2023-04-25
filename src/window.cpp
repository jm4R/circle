#include <circle/window.hpp>

#include <circle/private/draw_engine.hpp>

#include <circle/application.hpp>

#include <algorithm>
#include <cassert>

namespace circle {

window::window(unsigned w, unsigned h) : w_{w}, h_{h}
{
    assert(app);

    engine::window_init(ctx_, w_, h_);
    engine::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    engine::clear(ctx_);
    //content_item_.set_ctx(&ctx_);
    engine::set_event_handler(
        ctx_,
        [](void* data, const engine::event& ev) {
            static_cast<window*>(data)->handle_event(ev);
        },
        this);

    content_item().width = w;
    content_item().height = h;

    app->windows_.push_back(this);
}

window::~window()
{
    app->windows_.erase(
        std::remove(app->windows_.begin(), app->windows_.end(), this),
        app->windows_.end());
    engine::window_destroy(ctx_);
}

item& window::content_item()
{
    return content_item_;
}

void window::show()
{
    engine::window_show(ctx_, true);
    update();
}

void window::update()
{
    redraw();
    engine::flush(ctx_);
}

void window::redraw()
{
    engine::set_color(ctx_, 0xff, 0xff, 0xff, 0xff);
    engine::clear(ctx_);
    //content_item().render();
}

void window::handle_event(const engine::event& event)
{
    std::visit([this](const auto& e) { handle_event(e); }, event);
}

void window::handle_event(const engine::resize_event& event)
{
    w_ = event.width;
    h_ = event.height;
    content_item().width = w_;
    content_item().height = h_;
    update();
}

} // namespace circle
