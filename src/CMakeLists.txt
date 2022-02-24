add_library(circle-core STATIC
    circle.cpp
)

target_include_directories(circle-core PUBLIC
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
