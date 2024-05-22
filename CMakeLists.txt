cmake_minimum_required(VERSION 3.10)

project(windfnd)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(OpenSSL COMPONENTS Crypto SSL REQUIRED)
find_package(ftxui REQUIRED)

include_directories(include/)
include_directories(third-party/)

add_executable(windfnd src/main.cpp src/forecast.cpp src/weather_api.cpp src/render.cpp)

target_link_libraries(windfnd PRIVATE ftxui::screen ftxui::dom ftxui::component OpenSSL::SSL OpenSSL::Crypto)

target_compile_definitions(windfnd PUBLIC CPPHTTPLIB_OPENSSL_SUPPORT)

if (TESTS)
    find_package(GTest REQUIRED)

    # Add the test executable
    add_executable(ForecastTests test/test_forecast.cpp src/forecast.cpp src/weather_api.cpp)

    # Include directories for the test
    target_include_directories(ForecastTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

    # Link required libraries for the test
    target_link_libraries(ForecastTests PRIVATE gtest gtest_main)

    # copy over the files needed for tests
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/test.json
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
    file(COPY ${CMAKE_CURRENT_SOURCE_DIR}/test/search_location.json
         DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

    # Add tests
    add_test(NAME ForecastTests COMMAND ForecastTests)
endif()
