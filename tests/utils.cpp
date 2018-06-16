#include "catch.hpp"
#include "utils.h"

StdStreams redirect_std_streams() {
    // redirect cout
    auto cout_oss = new std::ostringstream();
    std::streambuf *cout_buf = std::cout.rdbuf();
    std::cout.rdbuf(cout_oss->rdbuf());

    // redirect cerr
    auto cerr_oss = new std::ostringstream();
    std::streambuf *cerr_buf = std::cerr.rdbuf();
    std::cerr.rdbuf(cerr_oss->rdbuf());

    return StdStreams {
        cout_buf,
        cerr_buf,
        cout_oss,
        cerr_oss
    };
}

void restore_std_streams(StdStreams streams) {
    // restore cout and cerr to their original buffer
    std::cout.rdbuf(streams.cout_buf);
    std::cerr.rdbuf(streams.cerr_buf);

    delete streams.cout_oss;
    delete streams.cerr_oss;
}
