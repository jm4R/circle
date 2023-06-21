#pragma once

#include <circle/color.hpp>
#include <circle/font.hpp>
#include <circle/item.hpp>

#include <string>

namespace circle {
class text : public item
{
public: /*properties*/
    circle::font font;
    property<circle::color> color;
    property<std::string> value;

public:
    explicit text();

protected:
    void draw(sdl::context ctx) override;

private:
    void recalculate_size();
    void render_text(sdl::context ctx);


private:
    sdl::context ctx_;

    property<unit> text_width_;
    property<unit> text_height_;

    bool dirty_{true};
};

} // namespace circle