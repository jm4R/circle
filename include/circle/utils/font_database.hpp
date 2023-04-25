#pragma once

#include <cstddef>

namespace circle {

class font_database
{
    // TODO
public:
    void add_file(const char* path) {}
    void add_memory(const std::byte* data, std::size_t length) {}
};

} // namespace circle