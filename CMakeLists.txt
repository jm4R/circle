cmake_minimum_required(VERSION 3.21)

project(circle VERSION 0.1 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)

include(CMakeDependentOption)
include(cmake/CPM.cmake)

CPMAddPackage("gh:jm4R/reactive-cpp#develop")

cmake_dependent_option(CIRCLE_TEST "Generate test target" ON "${PROJECT_IS_TOP_LEVEL}" OFF)
option(CIRCLE_COVERAGE "Generate the coverage report" OFF)

if (CIRCLE_COVERAGE)
    include(cmake/CodeCoverage.cmake)
endif()

add_subdirectory(src)

if (CIRCLE_TEST)
    add_subdirectory(test)
endif()