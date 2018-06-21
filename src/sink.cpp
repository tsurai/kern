#include "sink.h"
#include <iostream>
#include <syslog.h>

namespace Kern {
    void Sink::write_ext(const Metadata &, const char *msg) {
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

    SyslogSink::SyslogSink(const char *ident, int option, int facility) {
        openlog(ident, option, facility);
    }

    SyslogSink::~SyslogSink() {
        closelog();
    }

    int SyslogSink::kern_lvl_to_syslog_lvl(LogLevel level) {
        switch(level) {
            case LogLevel::Trace:
                return LOG_DEBUG;
            case LogLevel::Debug:
                return LOG_DEBUG;
            case LogLevel::Info:
                return LOG_INFO;
            case LogLevel::Warning:
                return LOG_WARNING;
            case LogLevel::Error:
                return LOG_ERR;
            case LogLevel::Fatal:
                return LOG_ALERT;
            default:
                return LOG_NOTICE;
        }
    }

    void SyslogSink::write_ext(const Metadata &meta, const char *msg) {
        int syslog_level = this->kern_lvl_to_syslog_lvl(meta.level);

        syslog(syslog_level, "%s", msg);
    }
}

