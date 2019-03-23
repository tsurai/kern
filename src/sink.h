#ifndef SINK_H
#define SINK_H

#include <fstream>

#include "info.h"

namespace kern {
    // Abstract Sink interface to be implemented for different output targets.
    class Sink {
    public:
        virtual ~Sink() { };

        Sink(Sink&&);
        Sink& operator=(Sink&&);

        // Prevent copy operations.
        Sink(const Sink &) = delete;
        Sink& operator=(const Sink &) = delete;

        virtual void write(const char *) = 0;

        // Optional extended write method with additional metadata.
        virtual void write_ext(const Metadata &, const char *);
    protected:
        Sink() { };
    };

    // Outputs messages to stdout
    class StdoutSink : public Sink {
    public:
        void write(const char *) override;
    };

    // Outputs messages to stderr
    class StderrSink : public Sink {
    public:
        void write(const char *) override;
    };

    // Outputs messages to a logfile
    class FileSink : public Sink {
    public:
        FileSink() = delete;
        explicit FileSink(const char *filename) : file(filename, std::ios::out | std::ios::app) { };

        void write(const char *) override;
    private:
        std::ofstream file;
    };

#ifdef __linux__
    // Outputs messages to the syslogd service on linux
    class SyslogSink : public Sink {
    public:
        SyslogSink() = delete;
        SyslogSink(const char *, int, int);
        ~SyslogSink();

        void write_ext(const Metadata &, const char *) override;
        void write(const char *) override { };
    private:
        int kern_lvl_to_syslog_lvl(LogLevel);
    };
#endif
}

#endif //SINK_H
