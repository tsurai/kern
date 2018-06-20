#include "sink.h"
#include <iostream>

namespace Kern {
    void Sink::write_ext(Metadata &, const char *msg) {
        this->write(msg);
    }

    void StdoutSink::write(const char *msg) {
        std::cout << msg << std::endl;
    }

    void StderrSink::write(const char *msg) {
        std::cerr << msg << std::endl;
    }

   void FileSink::write(const char *msg) {
        this->file << msg << std::endl;
   }
}

