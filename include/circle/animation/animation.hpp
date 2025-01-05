#pragma once

#include <circle/reactive/property.hpp>
#include <circle/object.hpp>

namespace circle {
class rectangle : public object
{
public: /*properties*/
    property<bool> running{true};

};

} // namespace circle