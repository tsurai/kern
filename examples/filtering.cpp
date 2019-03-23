#include "kern.h"
#include <cstring>

void foobar() {
    info("bar");

    debug("filtered");
    error("filtered");
}

int main(int, char **) {
    // filters all debug messages in the foobar function and all error messages
    // globaly
    kern::DispatchBuilder()
        .sink(std::make_unique<kern::StdoutSink>())
        .level(~kern::LogLevel::Error)
        .filter([](auto meta) {
            return !((strncmp(meta.function, "foobar", 6) == 0) && meta.level == kern::LogLevel::Debug);
        })
        .apply();

    info("foo");

    error("filtered");

    foobar();

    return 0;
}
