cmake_minimum_required(VERSION 3.10)

project(windfnd)

set(CMAKE_CXX_STANDARD 17)
set(BUILD_TESTING OFF)
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
    nlohman-json
    GIT_REPOSITORY https://github.com/nlohmann/json
    GIT_TAG develop
)
FetchContent_MakeAvailable(nlohman-json)

include_directories(include/)

add_executable(windfnd src/main.cpp)

target_link_libraries(windfnd PRIVATE ftxui::screen ftxui::dom ftxui::component nlohmann_json::nlohmann_json httplib::httplib)
