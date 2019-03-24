# DispatchBuilder
The DispatchBuilder is the interface to create and configure a kern logger. A branching method chaining model is used to allow the creation of complex, yet compact and easy to read logging configurations.

## Filtering
### ```level(LogLevel)```
Filters all messages with a LogLevel that's not included in the given Level bitmask. The following example only shows messages with a LogLevel of Debug or Info.
```c++
DispatchBuilder()
    .level(LogLevel::Debug | LogLevel::Info)
    .sink(std::make_unique<StdoutSink>())
    .apply();
```

### ```min_level(LogLevel)```
Filters all messages with a Level below the given argument.<br>
**Warning:** The argument **must** be a single Level in order to avoid undefined behavior.

The following example logs messages with the level Info, Warning, Error and Fatal.
```c++
DispatchBuilder()
    .min_level(LogLevel::Info)
    .sink(std::make_unique<StdoutSink>())
    .apply();
```

### ```max_level(LogLevel)```
The opposit version of ```min_level(LogLevel)``` filtering all messages above the given Level.<br>
**Warning:** The argument **must** be a single Level in order to avoid undefined behavior.

### ```filter(std::function<bool (const Metadata &)>)```
Filters messages based on the boolean return value of the function given by the user. The filtering function gets a Metadata struct containing the log level, level name, source file, source function and source file line as its argument.<br>
Returning ```true``` means the message should be processed. ```false``` means the message should be filtered.

The following example filtes all messages in the main.cpp source file.
```c++
DispatchBuilder()
    .sink(std::make_unique<StdoutSink>())
    .filter([](auto meta) {
        return !(strncmp(meta.src_file, "main.cpp", 8) == 0);
    })
    .apply();
```

### ```filter_chains()```
Filters all messages previously successfully processed by a chained Dispatch.

The following example prevents error messages to be delivered to Stdout additionally to Stderr.
```c++
DispatchBuilder()
    .sink(std::make_unique<StdoutSink>())
    .chain(DispatchBuilder()
        .level(LogLevel::Error)
        .sink(std::make_unique<StderrSink>())
        .build())
    .filter_chains()
    .apply();
```

## Formatting
### ```format(std::function<void (const Metadata &, const char *msg, char *buf)>)```
The format method allows direct manipulation of the message output buffer and thus the format in which the message should be outputted. The user supplied function gets three arguments. The previously described Metadata struct, the actual log message and the output buffer.

It is highly adviced to use ```snprintf``` with the ```BUF_SIZE``` supplied by kern to avoid buffer overflows. The default buffer size is 256 bytes.

```c++
kern::DispatchBuilder()
    .sink(std::make_unique<kern::StdoutSink>())
    .format([](auto meta, auto msg, auto buf) {
        snprintf(buf, BUF_SIZE, "[%s] %s:%d | %s", meta.level_str, meta.function, meta.line, msg);
    })
    .apply();
```

## Output
How messages are being outputted is determined by the Sink given to the Dispatch. The default behavior without supplying a Sink causes all messages to be dropped. Sinks have to be wrapped in a ```unique_ptr``` using the ```make_unique``` stdlib function.

Kern includes several different Sinks as well as an Interface to allow the implementation of custom Sinks.
```c++
DispatchBuilder()
    .sink(std::make_unique<StdoutSink>())
    .apply();
```

### ```Stdout()```
Outputs messages to the stdout attached to the running process.

### ```Stderr()```
Outputs messages to the stderr attached to the running process.

### ```SyslogSink(const char *ident, int option, int facility)```
Outputs messages to the syslog logging system. The three required arguments are the same as expected by the syslog ```openlog()``` function. Please consults the syslog MAN page for a detailed description of the possible values for these arguments.

### ```FileSink(const char *filename)```
Outputs messages to the given file. Kern will create the file if it does not already exist or open it in attach mode making sure that old messages won't be deleted.

## Chaining
Chaining is the most crucial feature to allow the construction of highly complex and customizable logging configurations. It essentially duplicates a message and passes it to another Dispatch for further processing. This enables various setups like special formatting for a specific LogLevel or the output of certain messages to multiple output targets.

### ```chain(std::unique_ptr<Dispatch>)```
The ```chain``` method expects a Dispatch wrapped in a ```unique_ptr``` as its only argument. Such an object can be built by the ```DispatchBuilder``` classes ```build()``` method.

```c++
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
```

## Finalizing

### ```apply()```
The ```apply()``` method has to be called on the outer most object in order to set the global ```Dispatch``` to the final logging configuration and make it available to the output macros.

```c++
DispatchBuilder()
    .sink(std::make_unique<StdoutSink>())
    // applies this configuration to the global scope
    .apply();
```
