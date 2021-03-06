#ifndef UTILS_H
#define UTILS_H

#include "sink.h"
#include <fstream>
#include <sstream>
#include <iostream>

class BufSink : public kern::Sink {
public:
    BufSink() = delete;
    BufSink(char *buf);

    // prevent copy operations
    BufSink(const BufSink&) = delete;
    BufSink operator=(const BufSink&) = delete;

    void write(const char *msg);
private:
    char *buf;
};


struct StdStreams {
    std::streambuf *cout_buf;
    std::streambuf *cerr_buf;
    std::ostringstream *cout_oss;
    std::ostringstream *cerr_oss;
};

StdStreams redirect_std_streams();
void restore_std_streams(StdStreams streams);

#endif //UTILS_H
