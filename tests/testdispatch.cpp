#include "dispatch.h"

#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "catch.hpp"
#include "kern.h"
#include "utils.h"

using namespace kern;

TEST_CASE("Dispatch filters by level") {
    char buf[16];
    buf[0] = '\0';

    const char *res = "[info] foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .level(LogLevel::Info)
        .apply();

    trace("foobar");
    CHECK( buf[0] == '\0' );

    debug("foobar");
    CHECK( buf[0] == '\0' );

    warning("foobar");
    CHECK( buf[0] == '\0' );

    error("foobar");
    CHECK( buf[0] == '\0' );

    fatal("foobar");
    CHECK( buf[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}

TEST_CASE("Dispatch filters by min_level") {
    char buf[16];
    buf[0] = '\0';

    const char *res_info = "[info] foobar";
    const char *res_warning = "[warning] foobar";
    const char *res_error = "[error] foobar";
    const char *res_fatal = "[fatal] foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .min_level(LogLevel::Info)
        .apply();

    trace("foobar");
    CHECK( buf[0] == '\0' );

    debug("foobar");
    CHECK( buf[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf, res_info, strlen(res_info)) == 0 );
    bzero((void *)buf, strlen(res_info));

    warning("foobar");
    CHECK( strncmp(buf, res_warning, strlen(res_warning)) == 0 );
    bzero((void *)buf, strlen(res_warning));

    error("foobar");
    CHECK( strncmp(buf, res_error, strlen(res_error)) == 0 );
    bzero((void *)buf, strlen(res_error));

    fatal("foobar");
    CHECK( strncmp(buf, res_fatal, strlen(res_fatal)) == 0 );
}

TEST_CASE("Dispatch filters by filter function") {
    char buf[16];
    buf[0] = '\0';

    const char *res = "[info] foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .filter([](auto meta) {
            return meta.level != LogLevel::Error;
        })
        .apply();

    error("foobar");
    CHECK( buf[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}

TEST_CASE("Dispatch formats the output message") {
    char buf[16];
    const char *res = "Foo:foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .format([](auto, auto msg, auto buf) {
            snprintf(buf, 16, "Foo:%s", msg);
        })
        .apply();

    info("foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}

TEST_CASE("Dispatch formats the input message") {
    char buf[32];
    const char *res = "[info] 42 4.2 foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .apply();

    info("%d %.1f %s", 42, 4.2, "foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}

TEST_CASE("Prevent reads to uninitialized buffer memory") {
    char buf[32];

    DispatchBuilder()
        .format([](auto, auto, auto) {})
        .sink(std::make_unique<BufSink>(buf))
        .apply();

    info("foobar");

    CHECK( buf[0] == '\0' );
}
