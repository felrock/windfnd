cc_library(
    name = "ftxui_screen",
    srcs = glob(["src/ftxui/screen/*.cpp"], exclude=["src/ftxui/screen/*test.cpp"]),
    hdrs = glob(["**/*.hpp"]),
    includes = ["include/", "src/"],
    visibility = ["//visibility:public"],
    copts = ["-std=c++17"],
)

cc_library(
    name = "ftxui_dom",
    srcs = glob(["src/ftxui/dom/*.cpp"], exclude=["src/ftxui/dom/*test.cpp"]),
    hdrs = glob(["**/*.hpp"]),
    includes = ["include/", "src/"],
    deps = [":ftxui_screen"],
    visibility = ["//visibility:public"],
    copts = ["-std=c++17"],
)
print(glob(["**/*.hpp"]))
cc_library(
    name = "ftxui_component",
    srcs = glob(["src/ftxui/component/*.cpp"], exclude=["src/ftxui/component/*test.cpp"]),
    hdrs = glob(["**/*.hpp"]),
    includes = ["include/", "src/"],
    deps = [":ftxui_screen", ":ftxui_dom"],
    visibility = ["//visibility:public"],
    copts = ["-std=c++17"],
)
