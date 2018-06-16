#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <sstream>
#include <iostream>

struct StdStreams {
    std::streambuf *cout_buf;
    std::streambuf *cerr_buf;
    std::ostringstream *cout_oss;
    std::ostringstream *cerr_oss;
};

StdStreams redirect_std_streams();
void restore_std_streams(StdStreams streams);

#endif //UTILS_H
