# Copyright 2016 The Bazel Authors. All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#    http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

# This becomes the BUILD file for @local_config_cc// under non-BSD unixes.

package(default_visibility = ["//visibility:public"])

load(":cc_toolchain_config.bzl", "cc_toolchain_config")
load(":armeabi_cc_toolchain_config.bzl", "armeabi_cc_toolchain_config")
load("@rules_cc//cc:defs.bzl", "cc_toolchain", "cc_toolchain_suite")

licenses(["notice"])  # Apache 2.0

cc_library(
    name = "malloc",
)

filegroup(
    name = "empty",
    srcs = [],
)

filegroup(
    name = "cc_wrapper",
    srcs = ["cc_wrapper.sh"],
)

filegroup(
    name = "compiler_deps",
    srcs = glob(["extra_tools/**"], allow_empty = True) + [":builtin_include_directory_paths"],
)

# This is the entry point for --crosstool_top.  Toolchains are found
# by lopping off the name of --crosstool_top and searching for
# the "${CPU}" entry in the toolchains attribute.
cc_toolchain_suite(
    name = "toolchain",
    toolchains = {
        "k8|compiler": ":cc-compiler-k8",
        "k8": ":cc-compiler-k8",
        "armeabi-v7a|compiler": ":cc-compiler-armeabi-v7a",
        "armeabi-v7a": ":cc-compiler-armeabi-v7a",
    },
)

cc_toolchain(
    name = "cc-compiler-k8",
    toolchain_identifier = "local",
    toolchain_config = ":local",
    all_files = ":compiler_deps",
    ar_files = ":compiler_deps",
    as_files = ":compiler_deps",
    compiler_files = ":compiler_deps",
    dwp_files = ":empty",
    linker_files = ":compiler_deps",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 1,
    module_map = None,
)

cc_toolchain_config(
    name = "local",
    cpu = "k8",
    compiler = "compiler",
    toolchain_identifier = "local",
    host_system_name = "local",
    target_system_name = "local",
    target_libc = "local",
    abi_version = "local",
    abi_libc_version = "local",
    cxx_builtin_include_directories = ["/nix/store/2khnb1660zpbz61k7jyz6sdqwdlm37zh-openjdk-headless-11.0.12+7/include",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0/include",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/include",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/lib/gcc/x86_64-unknown-linux-gnu/10.3.0/include-fixed",
    "/nix/store/hpbw53p17pld87yna45d6qrifxw8q7iq-glibc-2.33-55-dev/include",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/include/c++/10.3.0",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/include/c++/10.3.0/x86_64-unknown-linux-gnu",
    "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/include/c++/10.3.0/backward"],
    tool_paths = {"ar": "/nix/store/ffcrrgb6k5h7crr93dj90bi9fy1jvj16-binutils-2.35.1/bin/ar",
        "ld": "/nix/store/4g5fidyk19akng9n91cql60ylrjq93s6-gcc-wrapper-10.3.0/bin/ld",
        "cpp": "/nix/store/4g5fidyk19akng9n91cql60ylrjq93s6-gcc-wrapper-10.3.0/bin/cpp",
        "gcc": "/nix/store/4g5fidyk19akng9n91cql60ylrjq93s6-gcc-wrapper-10.3.0/bin/gcc",
        "dwp": "/nix/store/ffcrrgb6k5h7crr93dj90bi9fy1jvj16-binutils-2.35.1/bin/dwp",
        "gcov": "/nix/store/gbaf9hcffnrblpj7qv5lsrkcvjny08cc-gcc-10.3.0/bin/gcov",
        "nm": "/nix/store/ffcrrgb6k5h7crr93dj90bi9fy1jvj16-binutils-2.35.1/bin/nm",
        "objcopy": "/nix/store/ffcrrgb6k5h7crr93dj90bi9fy1jvj16-binutils-2.35.1/bin/objcopy",
        "objdump": "/nix/store/ffcrrgb6k5h7crr93dj90bi9fy1jvj16-binutils-2.35.1/bin/objdump",
        "strip": "/nix/store/4g5fidyk19akng9n91cql60ylrjq93s6-gcc-wrapper-10.3.0/bin/strip"},
    compile_flags = ["-U_FORTIFY_SOURCE",
    "-fstack-protector",
    "-Wall",
    "-Wunused-but-set-parameter",
    "-Wno-free-nonheap-object",
    "-fno-omit-frame-pointer"],
    opt_compile_flags = ["-g0",
    "-O2",
    "-D_FORTIFY_SOURCE=1",
    "-DNDEBUG",
    "-ffunction-sections",
    "-fdata-sections"],
    dbg_compile_flags = ["-g"],
    cxx_flags = ["-std=c++0x"],
    link_flags = ["-fuse-ld=gold",
    "-Wl,-no-as-needed",
    "-Wl,-z,relro,-z,now",
    "-B/nix/store/4g5fidyk19akng9n91cql60ylrjq93s6-gcc-wrapper-10.3.0/bin",
    "-pass-exit-codes",
    "-lstdc++",
    "-lm"],
    link_libs = [],
    opt_link_flags = ["-Wl,--gc-sections"],
    unfiltered_compile_flags = ["-fno-canonical-system-headers",
    "-Wno-builtin-macro-redefined",
    "-D__DATE__=\"redacted\"",
    "-D__TIMESTAMP__=\"redacted\"",
    "-D__TIME__=\"redacted\""],
    coverage_compile_flags = ["--coverage"],
    coverage_link_flags = ["--coverage"],
    supports_start_end_lib = True,
)

# Android tooling requires a default toolchain for the armeabi-v7a cpu.
cc_toolchain(
    name = "cc-compiler-armeabi-v7a",
    toolchain_identifier = "stub_armeabi-v7a",
    toolchain_config = ":stub_armeabi-v7a",
    all_files = ":empty",
    ar_files = ":empty",
    as_files = ":empty",
    compiler_files = ":empty",
    dwp_files = ":empty",
    linker_files = ":empty",
    objcopy_files = ":empty",
    strip_files = ":empty",
    supports_param_files = 1,
)

armeabi_cc_toolchain_config(name = "stub_armeabi-v7a")
