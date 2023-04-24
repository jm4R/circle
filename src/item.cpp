#include <circle/item.hpp>

namespace circle {

bool item::drawable() const
{
    return visible;
}

}