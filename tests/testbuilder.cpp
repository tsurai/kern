#include "builder.h"

#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "catch.hpp"
#include "kern.h"
#include "utils.h"


using namespace Kern;
using namespace Catch::Matchers;

TEST_CASE("DispatchBuilder doesn't accept invalid arguments") {
    auto b = new DispatchBuilder();

    CHECK_THROWS_AS( b->format(nullptr), std::invalid_argument );
    CHECK_THROWS_AS( b->sink(nullptr), std::invalid_argument );
    CHECK_THROWS_AS( b->chain(nullptr), std::invalid_argument );
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

TEST_CASE("DispatchBuilder inherents Dispatch properties to its chain") {
    char buf_parent[16];
    char buf_child[16];
    buf_child[0] = '\0';

    const char *res = "foobar";

    DispatchBuilder()
        .sink(std::make_unique<BufSink>(buf_parent))
        .format([](auto, auto msg, auto buf) {
            snprintf(buf, 16, "%s", msg);
        })
        .level(LogLevel::Info)
        .chain(DispatchBuilder()
            .sink(std::make_unique<BufSink>(buf_child))
            .build())
        .apply();

    error("foobar");
    CHECK( buf_child[0] == '\0' );

    info("foobar");
    CHECK( strncmp(buf_child, res, strlen(res)) == 0 );
}
