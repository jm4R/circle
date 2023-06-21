#pragma once

#include <circle/reactive/property.hpp>

namespace circle {

class font
{
public:
    enum capitalization_t
    {
        mixed_case,
        all_uppercase,
        all_lowercase,
        capitalize,
        // small_caps // not supported
    };

    enum hinting_t
    {
        hinting_normal,
        hinting_light,
        hinting_mono,
        hinting_none,
    };

    enum weight_t
    {
        thin = 100,
        light = 200,
        book = 300,
        normal = 400,
        medium = 500,
        demi_bold = 600,
        bold_ = 700,
        black = 800,
        fat = 900,
    };

public: /*properties*/
    property<std::string> family;
    property<bool> italic;
    property<bool> underline;
    property<bool> strikeout;
    property<bool> bold;
    property<weight_t> weight;
    property<int> point_size{10};

public: /*signals*/
    signal<const font&> value_changed;

public:
    explicit font();
};

} // namespace circle