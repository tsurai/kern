#include "builder.h"

#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "catch.hpp"
#include "kern.h"
#include "utils.h"

using namespace kern;
using namespace Catch::Matchers;

TEST_CASE("DispatchBuilder doesn't accept invalid arguments") {
    auto b = new DispatchBuilder();

    CHECK_THROWS_AS( b->format(nullptr), std::invalid_argument );
    CHECK_THROWS_AS( b->sink(nullptr), std::invalid_argument );
    CHECK_THROWS_AS( b->chain(nullptr), std::invalid_argument );
    CHECK_THROWS_AS( b->min_level(static_cast<LogLevel>(0)), std::invalid_argument );
    CHECK_THROWS_AS( b->min_level(LogLevel::Debug | LogLevel::Info), std::invalid_argument );
    CHECK_THROWS_AS( b->max_level(static_cast<LogLevel>(0)), std::invalid_argument );
    CHECK_THROWS_AS( b->max_level(LogLevel::Debug | LogLevel::Info), std::invalid_argument );
}

TEST_CASE("DispatchBuilder can't be reused") {
    auto b = new DispatchBuilder();

    CHECK_NOTHROW( b->build() );

    CHECK_THROWS_AS( b->build(), BuilderReuseException );
    CHECK_THROWS_AS( b->level(LogLevel::All), BuilderReuseException );
    CHECK_THROWS_AS( b->filter(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->format(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->sink(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->chain(nullptr), BuilderReuseException );
    CHECK_THROWS_WITH( b->apply(), Equals("Reuse of consumed builder") );
}

TEST_CASE("DispatchBuilder creates valid default object") {
    auto d = DispatchBuilder()
        .build();

    REQUIRE( d != nullptr );
}

TEST_CASE("DispatchBuilder inherits Dispatch properties to its chain") {
    char buf[16];
    buf[0] = '\0';

    const char *res = "foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf))
        .format([](auto, auto msg, auto buf) {
            snprintf(buf, 16, "%s", msg);
        })
        .chain(DispatchBuilder()
            .level(LogLevel::Info)
            .build())
        .apply();

    info("foobar");
    CHECK( strncmp(buf, res, strlen(res)) == 0 );
}
