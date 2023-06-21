#include <circle/font.hpp>

namespace circle {

font::font()
{
    // TODO: don't capture this
    auto changed = [this] { value_changed(*this); };
    family.value_changed().connect(changed);
    italic.value_changed().connect(changed);
    underline.value_changed().connect(changed);
    strikeout.value_changed().connect(changed);
    bold.value_changed().connect(changed);
    weight.value_changed().connect(changed);
    point_size.value_changed().connect(changed);
}

} // namespace circle