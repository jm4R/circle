#pragma once

#include <circle/item.hpp>

namespace circle
{
    class rectangle : public item
    {
    public:
        enum horizontal_mode
        {
            left,
            horizontal_center,
            right
        };

        enum vertical_mode
        {
            top,
            vertical_center,
            bottom
        };
    };

}