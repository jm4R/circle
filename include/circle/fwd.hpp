#include <circle/reactive/tracking_ptr.hpp>

namespace circle {
class object;
class item;
using item_ptr = tracking_ptr<item>;

using unit = int;

struct rect
{
    unit x;
    unit y;
    unit width;
    unit height;
};
} // namespace circle