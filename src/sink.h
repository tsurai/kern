#ifndef SINK_H
#define SINK_H

#include <fstream>

#include "info.h"

namespace Kern {
    class Sink {
    public:
        virtual ~Sink() { };

        Sink(Sink&&);
        Sink& operator=(Sink&&);

        // prevent copy operations
        Sink(const Sink &) = delete;
        Sink& operator=(const Sink &) = delete;

        virtual void write(const char *) = 0;
        virtual void write_ext(const Metadata &, const char *);
    protected:
        Sink() { };
    };


    class StdoutSink : public Sink {
    public:
        void write(const char *);
    };


    class StderrSink : public Sink {
    public:
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


#ifdef __linux__
    class SyslogSink : public Sink {
    public:
        SyslogSink() = delete;
        SyslogSink(const char *, int, int);
        ~SyslogSink();

        void write_ext(const Metadata &, const char *);
        void write(const char *) { };
    private:
        int kern_lvl_to_syslog_lvl(LogLevel);
    };
#endif
}

#endif //SINK_H
