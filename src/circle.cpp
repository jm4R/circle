#include <circle/reactive/property.hpp>

namespace circle
{
    class object
    {
        virtual bool drawable()
        {
            return false;
        }
    };

    class anchors : public item
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

    class item : public object
    {
    public:
        property<item&> parent;
    };
}