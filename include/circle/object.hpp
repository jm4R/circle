#pragma once

namespace circle {

class object
{
public:
    object() = default;
    virtual bool drawable() const;
};

} // namespace circle