#ifndef SINK_H
#define SINK_H

#include <fstream>
#include "info.h"

namespace Kern {
    class Sink {
    public:
        virtual ~Sink() { };
        Sink(const Sink &) = delete;
        Sink& operator=(const Sink &) = delete;

        virtual void write(const char *) = 0;
        virtual void write_ext(Metadata &, const char *);
    protected:
        Sink() { };
    };

    class StdoutSink : public Sink {
    public:
        StdoutSink() { };
        void write(const char *);
    };

    class StderrSink : public Sink {
    public:
        StderrSink() { };

        void write(const char *);
    };

    class FileSink : public Sink {
    public:
        FileSink() = delete;
        FileSink(const char *filename) : file(filename, std::ios::out | std::ios::app) { };
        void write(const char *);
    private:
        std::ofstream file;
    };
}

#endif //SINK_H
