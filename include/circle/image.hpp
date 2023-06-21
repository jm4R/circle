#pragma once

#include <circle/item.hpp>

#include <string>

namespace circle {
class image : public item
{
public: /*properties*/
    property<std::string> source;
    property<unit> source_width;
    property<unit> source_height;

public:
    explicit image();

protected:
    void draw(sdl::context ctx) override;

private:
    void load(sdl::context ctx);

private:
    sdl::context ctx_;

    property<unit> loaded_width_;
    property<unit> loaded_height_;

    bool dirty_{true};
};

} // namespace circle