#include "catch.hpp"
#include "kern.h"

#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "utils.h"

using namespace Kern;

TEST_CASE("Macros pass the correct level") {
    char buf[32];

    DispatchBuilder()
        .format([](auto meta, auto, auto buf) {
            snprintf(buf, 32, "%s", meta.level_str);
        })
        .sink(std::make_unique<BufSink>(buf))
        .apply();

    trace("");
    CHECK( strncmp(buf, "trace", 5) == 0 );

    debug("");
    CHECK( strncmp(buf, "debug", 5) == 0 );

    info("");
    CHECK( strncmp(buf, "info", 4) == 0 );

    warning("");
    CHECK( strncmp(buf, "warning", 7) == 0 );

    error("");
    CHECK( strncmp(buf, "error", 5) == 0 );

    fatal("");
    CHECK( strncmp(buf, "fatal", 5) == 0 );
}

TEST_CASE("Macros pass the source filename") {
    char buf[256];
    const char *res = "testmacros.cpp";

    DispatchBuilder()
        .format([](auto meta, auto, auto buf) {
            snprintf(buf, 256, "%s", meta.file);
        })
        .sink(std::make_unique<BufSink>(buf))
        .apply();

    info("");

    auto offset = strlen(buf) - strlen(res);
    CHECK( strncmp(buf+offset, res, strlen(res)) == 0 );
}

TEST_CASE("Macros pass the source code line") {
    int macro_line = 0;

    DispatchBuilder()
        .format([&macro_line](auto meta, auto, auto) {
            macro_line = meta.line;
        })
        .apply();

    int line = __LINE__; info("");

    CHECK( line == macro_line );
}

TEST_CASE("Macros pass the function name") {
    char buf[32];
    const char *res = "____C_A_T_C_H____";

    DispatchBuilder()
        .format([](auto meta, auto, auto buf) {
            snprintf(buf, 32, "%s", meta.function);
        })
        .sink(std::make_unique<BufSink>(buf))
        .apply();

    info("");

    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}
