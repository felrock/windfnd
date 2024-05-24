workspace(name = "windfnd")

# Add necessary external dependencies
load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")
load("@bazel_tools//tools/build_defs/repo:git.bzl", "git_repository")

# OpenSSL
http_archive(
    name = "openssl",
    urls = ["https://www.openssl.org/source/openssl-3.3.0.tar.gz"],
    strip_prefix = "openssl-3.3.0",
    build_file = "@//third-party:openssl.BUILD",
)

# FTXUI
git_repository(
    name = "ftxui",
    remote = "https://github.com/felrock/FTXUI.git",
    branch = "main",
    build_file = "@//third-party:ftxui.BUILD",
)

# Google Test
http_archive(
    name = "googletest",
    urls = ["https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz"],
    strip_prefix = "googletest-1.14.0",
)

load("@bazel_tools//tools/build_defs/repo:http.bzl", "http_archive")

http_archive(
    name = "rules_foreign_cc",
    # TODO: Get the latest sha256 value from a bazel debug message or the latest
    #       release on the releases page: https://github.com/bazelbuild/rules_foreign_cc/releases
    #
    # sha256 = "...",
    strip_prefix = "rules_foreign_cc-51152aac9d6d8b887802a47ec08a1a37ef2c4885",
    url = "https://github.com/bazelbuild/rules_foreign_cc/archive/51152aac9d6d8b887802a47ec08a1a37ef2c4885.tar.gz",
)

load("@rules_foreign_cc//foreign_cc:repositories.bzl", "rules_foreign_cc_dependencies")

rules_foreign_cc_dependencies()
