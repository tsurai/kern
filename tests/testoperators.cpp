#include "catch.hpp"
#include "kern.h"

using namespace kern;

TEST_CASE("bitwise operators calculate the correct value") {
    auto level = LogLevel::Trace | LogLevel::Debug;
    CHECK( (char)level == (1 << 0 | 1 << 1) );

    level = LogLevel::Trace & LogLevel::Debug;
    CHECK( (char)level == (1 << 0 & 1 << 1) );

    level = LogLevel::Trace ^ LogLevel::Debug;
    CHECK( (char)level == (1 << 0 ^ 1 << 1) );

    level = ~LogLevel::Trace;
    CHECK( (char)level == ~(1 << 0) );
}

TEST_CASE("compound assignment operations calculate the correct value") {
    auto level = LogLevel::Trace;
    level |= LogLevel::Debug;

    CHECK( (char)level == (1 << 0 | 1 << 1) );

    level = LogLevel::Trace;
    level &= LogLevel::Debug;

    CHECK( (char)level == (1 << 0 & 1 << 1) );
}
