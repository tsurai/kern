kern [![Build Status](https://travis-ci.org/tsurai/kern.svg?branch=master)](https://travis-ci.org/tsurai/kern) [![codecov](https://codecov.io/gh/tsurai/kern/branch/master/graph/badge.svg)](https://codecov.io/gh/tsurai/kern)
===
Simple C++ logging library using a branching method chaining interface inspired by the Rust crate [fern].

```rust
DispatchBuilder()
  .level(LogLevel::All)
  .format([](auto meta, auto msg, auto buf) {
    snprintf(buf, 265, "[%s] %s", meta.level_str, msg);
  })
  .chain(Logger::DispatchBuilder()
    .level(LogLevel::Error)
    .sink(std::make_unique<StderrSink>())
    .build())
  .apply();

info("foobar");
error("lorem ipsum");
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
