#pragma once

#include <circle/fwd.hpp>
#include <circle/utils/font_database.hpp>
#include <circle/utils/sdl_utils.hpp>

#include <vector>

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

    font_database& fonts();

protected:
    void handle_event(const sdl::event& ev);

private:
    font_database fonts_;
    std::vector<window*> windows_;
    bool quit_{false};

private:
    friend class window;
};

inline application* app{};

} // namespace circle