CPMAddPackage("gh:catchorg/Catch2@2.13.8")
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/contrib")

enable_testing()
include(Catch)

add_executable(circle-test
    anchors_test.cpp
    main_test.cpp
)

target_link_libraries(circle-test PRIVATE
    circle::core
    Catch2::Catch2
)

catch_discover_tests(circle-test)

if (REACTIVE_CPP_COVERAGE)
    add_gcovr_target_html(circle-coverage
        TEST_TARGET circle-test
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/include
    )

    add_gcovr_target_xml(circle-coverage-xml
        TEST_TARGET circle-test
        SOURCE_DIR ${PROJECT_SOURCE_DIR}/include
    )
endif()
