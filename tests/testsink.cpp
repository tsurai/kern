#include "kern.h"

#include <cstdio>
#include <cstring>
#include <stdio.h>

#include "catch.hpp"
#include "utils.h"

using namespace kern;

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
    char tmp_filename[] = "kern_testing_XXXXXX";

    // create tmp file name by creating a temp file and deleting it
    // immediately. Using tmpnam would safe this additional step but
    // it creates compiler warnings
    REQUIRE( mkstemp(tmp_filename) != -1 );
    remove(tmp_filename);

    new FileSink(tmp_filename);
    std::ifstream f(tmp_filename);

    CHECK( f.good() );

    // remove temporary
    remove(tmp_filename);
}

TEST_CASE("FileSink outputs to the given file") {
    const char *res = "foobar";
    char tmp_filename[] = "/tmp/kern_testing_XXXXXX";

    // create tmp file name by creating a temp file and deleting it
    // immediately. Using tmpnam would safe this additional step but
    // it creates compiler warnings
    REQUIRE( mkstemp(tmp_filename) != -1 );

    auto sink = new FileSink(tmp_filename);
    std::ifstream f(tmp_filename);

    REQUIRE( f.good() );

    sink->write("foobar");

    char buf[16];
    f.getline(buf, 16);
    CHECK( strncmp(buf, res, strlen(res)) == 0 );

    delete sink;

    // remove temporary
    remove(tmp_filename);
}
