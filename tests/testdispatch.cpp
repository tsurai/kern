#include "catch.hpp"
#include "kern.h"
#include "utils.h"
#include <cstdio>
#include <cstring>
#include <stdio.h>

using namespace Kern;

class BufSink : public Sink {
public:
    BufSink(char *buf) {
        this->buf = buf;
    };

    void write(const char *msg) {
        snprintf(buf, 256, "%s", msg);
    }
private:
    char *buf;
};

TEST_CASE("Dispatch filters messages") {
    char *buf = (char *)calloc(1, 16);
    REQUIRE( buf != nullptr );

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .filter([](auto meta) {
            return meta.level != LogLevel::Error;
        })
        .apply();

    error("foobar");
    CHECK( buf[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf, "[info] foobar", 13) == 0 );
}

TEST_CASE("Dispatch formats the output message") {
    char buf[16];
    const char *res = "Foo:foobar\n";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .format([](auto, auto msg, auto buf) {
            snprintf(buf, 16, "Foo:%s\n", msg);
        })
        .apply();

    info("foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}
