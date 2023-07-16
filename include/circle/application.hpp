#pragma once

#include <circle/fwd.hpp>
#include <circle/object.hpp>
#include <circle/utils/font_database.hpp>
#include <circle/utils/sdl_utils.hpp>
#include <circle/event.hpp>

#include <list>
#include <vector>

namespace circle {

class application_base
{
public:
    explicit application_base();
    ~application_base();
};

class application : private application_base
{
public:
    application();
    ~application();

    application& operator=(application&&) = delete;

    int exec();
    void quit();

    void register_event_handler(object_ptr obj);
    bool post_event(event ev);

    font_database& fonts();

protected:
    void handle_event(const event& ev);

private:
    font_database fonts_;
    std::vector<window*> windows_;
    std::vector<object_ptr> event_handlers_;
    bool quit_{false};

private:
    friend class window;
};

inline application* app{};

} // namespace circle