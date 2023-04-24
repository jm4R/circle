set(circle_include_dir ${circle_SOURCE_DIR}/include/circle)

add_library(circle-core STATIC
    anchors.cpp
    item.cpp
    object.cpp
    rectangle.cpp

    ${circle_include_dir}/anchors.hpp
    ${circle_include_dir}/item.hpp
    ${circle_include_dir}/object.hpp
    ${circle_include_dir}/rectangle.hpp
    ${circle_include_dir}/fwd.hpp
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
)

add_library(circle::core ALIAS circle-core)
