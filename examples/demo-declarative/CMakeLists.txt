add_executable(circle-demo-declarative)
target_compile_features(circle-demo-declarative PUBLIC cxx_std_17)

target_sources(circle-demo-declarative PRIVATE
    main.cpp
)

target_link_libraries(circle-demo-declarative PUBLIC
    circle::core
)

if (EMSCRIPTEN)
    set_target_properties(circle-demo-declarative
        PROPERTIES
            SUFFIX ".html"
    )
endif()
