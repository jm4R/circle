#include <circle/utils/font_database.hpp>

#include <private/circle_sdl.hpp>

#include <algorithm>
#include <cassert>
#include <cmath>

namespace circle {

namespace {
struct weight
{
    const char* name;
    font::weight_t value;
};

// NOTE: do not reorder this array
weight weights[] = {{"thin", font::thin},
                    {"hairline", font::thin},
                    {"ultra light", font::thin},
                    {"extra light", font::thin},
                    {"light", font::light},
                    {"book", font::book},
                    {"regular", font::normal},
                    {"normal", font::normal},
                    {"plain", font::normal},
                    {"roman", font::normal},
                    {"standard", font::normal},
                    {"medium", font::medium},
                    {"semi bold", font::demi_bold},
                    {"demi bold", font::demi_bold},
                    {"ultra black", font::fat},
                    {"extra black", font::fat},
                    {"ultra bold", font::fat},
                    {"heavy black", font::fat},
                    {"fat", font::fat},
                    {"poster", font::fat},
                    {"heavy", font::black},
                    {"black", font::black},
                    {"extra bold", font::black},
                    {"bold", font::bold_}};

} // namespace

font_database::~font_database()
{
    for (auto& family : cache_)
    {
        for (auto& face : family.second.cached_faces)
            sdl::font_destroy(face.context);
    }
}

void font_database::add_file(const char* path)
{
    sdl::font_context font;
    sdl::font_init(font, path, 12);
    if (!font.font)
        return; // TODO< log it, don't use context internal member
    style s = get_style_info(font);
    s.file_path = path;
    add(font, s);
    sdl::font_destroy(font);
}

void font_database::add_memory(const std::byte* data, std::size_t length)
{
    sdl::font_context font;
    sdl::font_init_from_memory(font, data, length, 12);
    if (!font.font)
        return; // TODO< log it, don't use context internal member
    style s = get_style_info(font);
    s.memory_data = data;
    s.data_length = length;
    add(font, s);
    sdl::font_destroy(font);
}

void font_database::set_emergency_familly(std::string name)
{
    std::transform(name.begin(), name.end(), name.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    emergency_family_ = std::move(name);
}

sdl::font_context font_database::get(const font& f)
{
    assert(!cache_.empty());
    auto family_name = *f.family;
    std::transform(family_name.begin(), family_name.end(), family_name.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    const auto family_it = cache_.find(family_name);

    if (family_it == cache_.end())
        return get_emergency(f);
    else
        return find_matching_face(family_it->second, f);
}

font_database::style font_database::get_style_info(sdl::font_context& font)
{
    style result;

    std::string style_name = sdl::font_get_style_name(font);
    std::transform(
        style_name.begin(), style_name.end(), style_name.begin(),
        [](unsigned char c) { return c == '-' ? ' ' : std::tolower(c); });

    result.weight = font::normal;
    for (const auto& w : weights)
    {
        if (style_name.find(w.name) != std::string::npos)
        {
            result.weight = w.value;
            break;
        }
    }

    result.possible_disabled_styles =
        sdl::font_set_style(font, sdl::font_style(0));
    result.possible_enabled_styles = sdl::font_set_style(
        font, sdl::font_style(sdl::font_style_bold | sdl::font_style_italic |
                              sdl::font_style_underline |
                              sdl::font_style_strikethrough));

    return result;
}

void font_database::add(sdl::font_context font, style value)
{
    std::string family_name = sdl::font_get_family_name(font);
    std::transform(family_name.begin(), family_name.end(), family_name.begin(),
                   [](unsigned char c) { return std::tolower(c); });
    cache_[family_name].available_styles.push_back(std::move(value));
}

sdl::font_context font_database::get_emergency(const font& f)
{
    auto family_it = cache_.find(emergency_family_);
    if (family_it == cache_.end())
        family_it = cache_.begin();
    return find_matching_face(family_it->second, f);
}

sdl::font_context font_database::find_matching_face(family& family,
                                                    const font& f)
{
    int flags{};
    if (f.italic)
        flags |= sdl::font_style_italic;
    if (f.underline)
        flags |= sdl::font_style_underline;
    if (f.strikeout)
        flags |= sdl::font_style_strikethrough;
    if (f.bold)
        flags |= sdl::font_style_bold;

    style* best_match = &family.available_styles.front();
    int match_level = 0;
    // 0 = any from face
    // 1 = all required flags available
    // 2 = 1 && required flags can't be disabled (eg. take bold & italic font
    // form BoldItalic ttf) 3 = 2 && weight as close as possible (eg. take thin
    // font from Thin ttf)
    for (auto& entry : family.available_styles)
    {
        if (match_level < 1 && (entry.possible_enabled_styles & flags) == flags)
        {
            best_match = &entry;
            match_level = 1;
        }
        else
            continue;

        if (match_level < 2 && entry.possible_disabled_styles == flags)
        {
            best_match = &entry;
            match_level = 2;
        }
        else
            continue;

        if (match_level >= 2)
        {
            const int best_weight_diff =
                std::fabs(best_match->weight - f.weight);
            const int current_weight_diff = std::fabs(entry.weight - f.weight);
            if (current_weight_diff < best_weight_diff)
            {
                best_match = &entry;
                // match_level = 3;

                if (current_weight_diff == 0)
                    break; // perfect match
            }
        }
    }

    flags &= best_match->possible_enabled_styles;

    const auto found = std::find_if(
        family.cached_faces.begin(), family.cached_faces.end(),
        [&](const face& candidate) {
            return candidate.style ==
                       (best_match->possible_enabled_styles & flags) &&
                   candidate.weight == best_match->weight &&
                   candidate.size == f.point_size;
        });
    if (found != family.cached_faces.end())
        return found->context;

    face new_face{};
    if (best_match->memory_data)
        sdl::font_init_from_memory(new_face.context, best_match->memory_data,
                                   best_match->data_length, f.point_size);
    else
        sdl::font_init(new_face.context, best_match->file_path.c_str(),
                       f.point_size);

    new_face.style = sdl::font_set_style(new_face.context,
                                         static_cast<sdl::font_style>(flags));

    new_face.weight = best_match->weight;
    new_face.size = f.point_size;

    family.cached_faces.push_back(new_face);

    return new_face.context;
}

} // namespace circle