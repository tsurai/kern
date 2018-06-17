#include "catch.hpp"
#include "kern.h"
#include "utils.h"
#include <cstdio>
#include <stdio.h>

using namespace Kern;

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
    CHECK_THROWS_AS( b->format(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->sink(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->chain(nullptr), BuilderReuseException );
    CHECK_THROWS_AS( b->apply(), BuilderReuseException );

}

TEST_CASE("DispatchBuilder creates valid default object") {
    auto d = DispatchBuilder()
        .build();

    REQUIRE( d != nullptr );
}

TEST_CASE("DispatchBuilder can set the global logger") {
    DispatchBuilder()
        .apply();

    auto streams = redirect_std_streams();

    info("foobar");
    CHECK( streams.cout_oss->str() == "[info] foobar\n" );

    restore_std_streams(streams);
}
