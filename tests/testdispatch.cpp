#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "catch.hpp"
#include "kern.h"
#include "utils.h"

using namespace kern;

const char *res_trace = "[trace] foobar";
const char *res_debug = "[debug] foobar";
const char *res_info = "[info] foobar";
const char *res_warning = "[warning] foobar";
const char *res_error = "[error] foobar";
const char *res_fatal = "[fatal] foobar";

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

TEST_CASE("Dispatch filters by max_level") {
    char buf[16];
    buf[0] = '\0';

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .max_level(LogLevel::Info)
        .apply();

    trace("foobar");
    CHECK( strncmp(buf, res_trace, strlen(res_trace)) == 0 );
    bzero((void *)buf, strlen(res_trace));

    debug("foobar");
    CHECK( strncmp(buf, res_debug, strlen(res_debug)) == 0 );
    bzero((void *)buf, strlen(res_debug));

    info("foobar");
    CHECK( strncmp(buf, res_info, strlen(res_info)) == 0 );
    bzero((void *)buf, strlen(res_info));

    warning("foobar");
    CHECK( buf[0] == '\0' );

    error("foobar");
    CHECK( buf[0] == '\0' );

    fatal("foobar");
    CHECK( buf[0] == '\0' );
}

TEST_CASE("Dispatch filters by filter function") {
    char buf[16];
    buf[0] = '\0';

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .filter([](auto meta) {
            return meta.level != LogLevel::Error;
        })
        .apply();

    error("foobar");
    CHECK( buf[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf, res_info, strlen(res_info)) == 0 );
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

TEST_CASE("Dispatch filters chained messages") {
    char buf_debug[16];
    buf_debug[0] = '\0';

    char buf_info[16];
    buf_info[0] = '\0';

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf_info))
        .chain(DispatchBuilder()
            .level(LogLevel::Debug)
            .sink(std::make_unique<BufSink>(buf_debug))
            .build())
        .filter_chains()
        .apply();

    debug("foobar");
    CHECK( buf_info[0] == '\0' );
    CHECK( strncmp(buf_debug, res_debug, strlen(res_debug)) == 0 );

    info("foobar");
    CHECK( strncmp(buf_info, res_info, strlen(res_info)) == 0 );
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
