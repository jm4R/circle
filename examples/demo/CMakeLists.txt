add_executable(circle-demo)
target_compile_features(circle-demo PUBLIC cxx_std_17)

target_sources(circle-demo PRIVATE
    main.cpp
)

target_link_libraries(circle-demo PUBLIC
    circle::core
)
