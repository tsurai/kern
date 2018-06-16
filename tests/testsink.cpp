#include "catch.hpp"
#include "sink.h"
#include "utils.h"
#include <cstdio>
#include <stdio.h>

using namespace Kern;

TEST_CASE("StdoutSink outputs to stdout only") {
    auto streams = redirect_std_streams();

    auto s = new StdoutSink();
    s->write("foobar");

    CHECK( streams.cout_oss->str() == "foobar\n" );
    CHECK( streams.cerr_oss->str() != "foobar\n" );

    restore_std_streams(streams);
}

TEST_CASE("StderrSink outputs to stderr only") {
    auto streams = redirect_std_streams();

    auto s = new StderrSink();
    s->write("foobar");

    CHECK( streams.cerr_oss->str() == "foobar\n" );
    CHECK( streams.cout_oss->str() != "foobar\n" );

    restore_std_streams(streams);
}

TEST_CASE("FileSink creates file") {
    // create temp file name
    std::string filename = std::tmpnam(nullptr);
    const char *cfilename = filename.c_str();

    new FileSink(cfilename);
    std::ifstream f(cfilename);

    CHECK( f.good() );

    // remove temporary
    remove(cfilename);
}
