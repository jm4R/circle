#include <circle/application.hpp>

#include <circle/private/draw_engine.hpp>
#include "private/standard_fonts/standard_fonts.hpp"

#include <cassert>

namespace circle {

application_base::application_base()
{
    static bool once_guard{};

    assert(!once_guard);
    once_guard = true;

    engine::init_engine();
}

application_base::~application_base()
{
    engine::destroy_engine();
}

application::application()
{
    auto* r = Roboto_Regular_ttf;

    fonts_.add_memory(reinterpret_cast<std::byte*>(Roboto_Regular_ttf),
                      Roboto_Regular_ttf_len);
    app = this;
}

application::~application()
{
    app = nullptr;
}

int application::exec()
{
    while(!quit_){
        auto ev = engine::wait_for_event();
        std::visit([this](const auto& e) { handle_event(e); }, ev);
    }
    return 0;
}

void application::quit()
{
    quit_ = true; // FIXME push it to event queue
}

font_database& application::fonts()
{
    return fonts_;
}

void application::handle_event(const engine::resize_event &event)
{

}

void application::handle_event(const engine::quit_event &event)
{
    quit_ = true;
}

} // namespace circle
