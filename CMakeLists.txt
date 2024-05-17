cmake_minimum_required(VERSION 3.10)

project(windfnd)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TESTING ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
#set(CMAKE_BUILD_TYPE Debug)

include(FetchContent)

# Add a terminal ui lib
FetchContent_Declare(
    ftxui
    GIT_REPOSITORY https://github.com/ArthurSonzogni/FTXUI
    GIT_TAG main
)
FetchContent_MakeAvailable(ftxui)

# Add a http lib
FetchContent_Declare(
    cpp-httplib
    GIT_REPOSITORY https://github.com/yhirose/cpp-httplib
    GIT_TAG master
)
FetchContent_MakeAvailable(cpp-httplib)

# Add a json parser lib
FetchContent_Declare(
    nlohmann-json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG develop
)
FetchContent_MakeAvailable(nlohmann-json)

# Add GoogleTest
FetchContent_Declare(
    googletest
    GIT_REPOSITORY https://github.com/google/googletest
    GIT_TAG release-1.11.0
)
FetchContent_MakeAvailable(googletest)

include_directories(include/)

add_executable(windfnd src/main.cpp src/forecast.cpp src/weather_api.cpp src/render.cpp)

target_link_libraries(windfnd PRIVATE ftxui::screen ftxui::dom ftxui::component nlohmann_json::nlohmann_json httplib::httplib)

# Add the test executable
add_executable(ForecastTests test/test_forecast.cpp src/forecast.cpp src/weather_api.cpp)

# Include directories for the test
target_include_directories(ForecastTests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# Link required libraries for the test
target_link_libraries(ForecastTests PRIVATE gtest gtest_main nlohmann_json::nlohmann_json httplib::httplib)

# Add tests
add_test(NAME ForecastTests COMMAND ForecastTests)

