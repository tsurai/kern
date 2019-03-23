#include "kern.h"
#include <syslog.h>

int main(int, char **) {
    kern::DispatchBuilder()
        .sink(std::make_unique<kern::StdoutSink>())
        // write error messages to stderr...
        .chain(kern::DispatchBuilder()
            .min_level(kern::LogLevel::Error)
            .sink(std::make_unique<kern::StderrSink>())
            // ...and to a error log file
            .chain(kern::DispatchBuilder()
                .sink(std::make_unique<kern::FileSink>("/tmp/kern_err_log"))
                .build())
            .build())
        // add additional meta data to trace messeges
        .chain(kern::DispatchBuilder()
            .level(kern::LogLevel::Trace)
            .format([](auto meta, auto msg, auto buf) {
                snprintf(buf, 256, "[%s] %s:%d | %s", meta.level_str, meta.function, meta.line, msg);
            })
            .build())
        // filter all messages previously delivered to a chain dispatch
        .filter_chains()
        .apply();

    info("foo");
    error("bar");
    trace("baz");

    return 0;
}
