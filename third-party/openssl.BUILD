load("@rules_foreign_cc//foreign_cc:defs.bzl", "configure_make", "configure_make_variant")

filegroup(
    name = "all_srcs",
    srcs = glob(["**"]),
    visibility = ["//visibility:public"],
)

configure_make(
    name = "openssl",
    lib_source = ":all_srcs",
    out_shared_libs = ["libssl.so","libcrypto.so"],
    visibility = ["//visibility:public"],
    configure_command = "Configure",
    configure_options = ["-fPIC","-shared"],
    configure_in_place = True,
    out_lib_dir = "lib64",
    args = ["-j 8"],
)

cc_binary(
    name = "openssl_placeholder",
    srcs = ["src/test_openssl.cpp"],
    visibility = ["//visibility:public"],
    deps = [
        "//:openssl",
    ],
)
