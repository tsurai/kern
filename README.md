kern [![Build Status](https://travis-ci.org/tsurai/kern.svg?branch=master)](https://travis-ci.org/tsurai/kern) [![codecov](https://codecov.io/gh/tsurai/kern/branch/master/graph/badge.svg)](https://codecov.io/gh/tsurai/kern)
===
Simple C++ logging library using a branching method chaining interface inspired by the Rust crate [fern].

```c++
#include <kern/kern.h>

DispatchBuilder()
    // accept messages for all level
    .level(LogLevel::All)
    // prefix every log message with its level
    .format([](auto meta, auto msg, auto buf) {
        snprintf(buf, 256, "[%s] %s", meta.level_str, msg);
    })
    .chain(DispatchBuilder()
        .level(LogLevel::Trace)
        // use additional debug informations for trace messages
        .format([](auto meta, auto msg, auto buf) {
            snprintf(buf, 256, "[%s] %s:%d | %s", meta.level_str, meta.function, meta.line, msg);
        })
        .sink(std::make_unique<FileSink>("/var/log/foobar.trace"))
        .build())
    // write error messages to stdout and stderr
    .chain(DispatchBuilder()
        .level(LogLevel::Error)
        .sink(std::make_unique<StderrSink>())
        // additionaly write error messages into a log file
        .chain(DispatchBuilder()
            .sink(std::make_unique<FileSink>("/var/log/foobar.err"))
            .build())
        .build())
    // send all other messages to stdout
    .chain(DispatchBuilder()
        .level(~(LogLevel::Trace | LogLevel::Error))
        .sink(std::make_unique<StdoutSink>())
        .build())
    .apply();

info("foo");
error("bar");
trace("bza");
```

### Todo

* [x] thread safety
* [ ] example code

### Testing

kern is using parts of the [catch] unit-testing framework for internal testing. All necessary files are beings
distributed with kern and can be found in the **tests/catch** subdirectory. Catch is licensed under the Boost
Software License 1.0

[fern]: https://github.com/daboross/fern
[catch]: https://github.com/catchorg/Catch2
