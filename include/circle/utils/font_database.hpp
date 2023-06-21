#pragma once

#include <circle/font.hpp>
#include <circle/utils/sdl_utils.hpp>

#include <cstddef>
#include <string>
#include <unordered_map>
#include <vector>

namespace circle {

class font_database
{
    /* example structure:
     *
     * "roboto": {
     *     available_styles: [
     *         {
     *             file_path: "/fonts/Roboto-Regular.txt"
     *             weight: regular
     *             possible_enabled_styles: bold | italic | underline
     *             possible_disabled_styles: bold | italic | underline
     *         },
     *         {
     *             file_path: "Roboto-BoldItalic.txt"
     *             weight: demi_bold
     *             possible_enabled_styles: bold | italic | underline
     *             possible_disabled_styles: underline
     *         }
     *     ]
     *
     *     cached_faces: [
     *         {
     *             weight: demi_bold
     *             style: underline
     *             size: 12
     *             value: font
     *         }
     *     ]
     * }
     *
     **/

    struct face
    {
        font::weight_t weight;
        sdl::font_style style;
        int size;
        sdl::font_context context;
    };

    struct style
    {
        std::string file_path{};
        const std::byte* memory_data{};
        std::size_t data_length{};
        font::weight_t weight;
        sdl::font_style possible_enabled_styles;
        sdl::font_style possible_disabled_styles;
    };
    struct family
    {
        std::vector<style> available_styles;
        std::vector<face> cached_faces;
    };
    using cache = std::unordered_map<std::string, family>;

public:
    explicit font_database() = default;
    ~font_database();

    font_database(const font_database&) = delete;
    font_database& operator=(const font_database&) = delete;
    font_database(font_database&&) = default;
    font_database& operator=(font_database&&) = default;

    void add_file(const char* path);
    void add_memory(const std::byte* data, std::size_t length);

    void set_emergency_familly(std::string name);

    sdl::font_context get(const font& f);

private:
    style get_style_info(sdl::font_context& font);
    void add(sdl::font_context font, style value);
    sdl::font_context get_emergency(const font& f);
    sdl::font_context find_matching_face(family& family, const font& f);

private:
    cache cache_;
    std::string emergency_family_;
};
} // namespace circle