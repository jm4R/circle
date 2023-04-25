#pragma once

#include <circle/fwd.hpp>
#include <circle/utils/font_database.hpp>

#include <vector>

#include <vector>

namespace circle {

namespace engine {
struct resize_event;    // TODO: temp
struct quit_event;
} // namespace engine

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

    font_database& fonts();

private:
    void handle_event(const engine::resize_event& event);
    void handle_event(const engine::quit_event& event);

private:
    font_database fonts_;
    std::vector<window*> windows_;
    bool quit_{false};

private:
    friend class window;
};

inline application* app{};

} // namespace circle