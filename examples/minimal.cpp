#include "kern.h"

int main(int, char **) {
    // output all messages to stdout using the default formatting
    kern::DispatchBuilder()
        .sink(std::make_unique<kern::StdoutSink>())
        .apply();

    // [debug] foo
    debug("foo");
    // [info] bar
    info("bar");
    // [warning] baz
    warning("baz");

    return 0;
}
