# kern [![Build Status](https://travis-ci.org/tsurai/kern.svg?branch=master)](https://travis-ci.org/tsurai/kern) [![Build status](https://ci.appveyor.com/api/projects/status/6juby5hp048r8xfk?svg=true)](https://ci.appveyor.com/project/tsurai/kern) [![codecov](https://codecov.io/gh/tsurai/kern/branch/master/graph/badge.svg)](https://codecov.io/gh/tsurai/kern)
Simple C++ logging library using a branching method chaining interface inspired by the Rust crate [fern].

```c++
#include <kern/kern.h>

DispatchBuilder()
    // accept messages for all level
    .sink(std::make_unique<StdoutSink>())
    .chain(DispatchBuilder()
        .level(LogLevel::Trace)
        // use additional debug informations for trace messages
        .format([](auto meta, auto msg, auto buf) {
            snprintf(buf, BUF_SIZE, "[%s] %s:%d | %s", meta.level_str, meta.function, meta.line, msg);
        })
        .sink(std::make_unique<FileSink>("/var/log/foobar.trace"))
        .build())
    // write error and fatal messages to stdout and stderr
    .chain(DispatchBuilder()
        .min_level(LogLevel::Error)
        .sink(std::make_unique<StderrSink>())
        // additionaly write error messages into a log file
        .chain(DispatchBuilder()
            .sink(std::make_unique<FileSink>("/var/log/foobar.err"))
            .build())
        .build())
    // filter all messages previously caught by a chain dispatch
    .filter_chains()
    .apply();

info("foo");
error("bar");
trace("baz %d %s", 6, "foobar");
```

## How to use
* Tutorial
* [Reference](docs/builder.md)

## Testing

kern is using parts of the [catch] unit-testing framework for internal testing. All necessary files are being
distributed with kern and can be found in the **tests/catch** subdirectory. Catch is licensed under the Boost
Software License 1.0

[fern]: https://github.com/daboross/fern
[catch]: https://github.com/catchorg/Catch2
