#include "kern.h"
#include <syslog.h>

int main(int, char **) {
    kern::DispatchBuilder()
        // send all messages to stdout
        .sink(std::make_unique<kern::StdoutSink>())
        // additionally write error and fatal messages into a logfile...
        .chain(kern::DispatchBuilder()
            .min_level(kern::LogLevel::Error)
            .sink(std::make_unique<kern::FileSink>("/tmp/kern_err_log"))
            // ...and to syslog
            .chain(kern::DispatchBuilder()
                .sink(std::make_unique<kern::SyslogSink>("kern", LOG_NDELAY, LOG_USER))
                .build())
            .build())
        .apply();

    info("foo");
    error("bar");
    fatal("baz");

    return 0;
}
