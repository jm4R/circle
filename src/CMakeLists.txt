set(circle_include_dir ${circle_SOURCE_DIR}/include/circle)

add_library(circle-core STATIC
    anchors.cpp
    application.cpp
    color.cpp
    column.cpp
    event.cpp
    font.cpp
    gradient.cpp
    image.cpp
    item.cpp
    mouse_area.cpp
    object.cpp
    rectangle.cpp
    row.cpp
    text.cpp
    timer.cpp
    window.cpp

    utils/font_database.cpp
    utils/sdl_utils.cpp

    private/circle_sdl.hpp private/circle_sdl.cpp
    private/standard_fonts/Roboto-Bold.ttf.cpp
    private/standard_fonts/Roboto-BoldItalic.ttf.cpp
    private/standard_fonts/Roboto-Italic.ttf.cpp
    private/standard_fonts/Roboto-Regular.ttf.cpp
    private/standard_fonts/standard_fonts.hpp

    ${circle_include_dir}/anchors.hpp
    ${circle_include_dir}/application.hpp
    ${circle_include_dir}/color.hpp
    ${circle_include_dir}/column.hpp
    ${circle_include_dir}/declarative.hpp
    ${circle_include_dir}/event.hpp
    ${circle_include_dir}/font.hpp
    ${circle_include_dir}/fwd.hpp
    ${circle_include_dir}/gradient.hpp
    ${circle_include_dir}/image.hpp
    ${circle_include_dir}/item.hpp
    ${circle_include_dir}/mouse_area.hpp
    ${circle_include_dir}/object.hpp
    ${circle_include_dir}/rectangle.hpp
    ${circle_include_dir}/row.hpp
    ${circle_include_dir}/text.hpp
    ${circle_include_dir}/timer.hpp
    ${circle_include_dir}/window.hpp

    ${circle_include_dir}/utils/font_database.hpp
    ${circle_include_dir}/utils/sdl_utils.hpp
)

target_include_directories(circle-core SYSTEM PUBLIC
    ${circle_SOURCE_DIR}/include
)

target_include_directories(circle-core PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_features(circle-core PUBLIC
    cxx_std_17
)

target_link_libraries(circle-core PUBLIC
    circle::reactive
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_ttf::SDL2_ttf
)

if (EMSCRIPTEN)
    target_link_options(circle-core PUBLIC "-sASYNCIFY")
endif()

add_library(circle::core ALIAS circle-core)
