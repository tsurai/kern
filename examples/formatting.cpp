#include "kern.h"

int main(int, char **) {
    // outputs all messages using a custom formatting with additional metadata
    kern::DispatchBuilder()
        .sink(std::make_unique<kern::StdoutSink>())
        .format([](auto meta, auto msg, auto buf) {
            snprintf(buf, kern::BUF_SIZE, "[%s] %s:%d | %s", meta.level_str, meta.function, meta.line, msg);
        })
        .apply();

    // [info] main:11 | bar
    info("bar");

    return 0;
}
