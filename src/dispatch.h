#ifndef DISPATCH_H
#define DISPATCH_H

#include <functional>
#include <memory>
#include <mutex>
#include <vector>

#include "info.h"
#include "sink.h"

// Necessary to properly export and import all symbols on Windows.
#ifdef _WIN32
    #ifdef BUILD_DLL
        #define DECLSPEC __declspec(dllexport)
    #else
        #define DECLSPEC __declspec(dllimport)
    #endif
#else
    #define DECLSPEC
#endif

namespace kern {
    // Size of the internal format and output buffer
    constexpr size_t buf_size = 256;

    // Allows direct control of how a log message should be formatted inside
    // of the internal buffer before writing it out to the output Sink.
    typedef std::function<void (const Metadata &, const char *, char *)> FnFormat;

    // Allows log messages to be filtered based on metadata before being
    // passed to the output Sink.
    typedef std::function<bool (const Metadata &)> FnFilter;

    // Formats and dispatches log messages to its output Sink and children.
    // Dispatch objects can not be created directly but use the DispatchBuilder
    // class for construction.
    class DECLSPEC Dispatch {
        // Grants DispatchBuilder full access for building Dispatch objects.
        friend class DispatchBuilder;
    public:
        ~Dispatch();

        Dispatch(Dispatch&&);
        Dispatch& operator=(Dispatch&&);

        // Prevent copy operations.
        Dispatch(const Dispatch &) = delete;
        Dispatch& operator=(const Dispatch &) = delete;

        // Processes the raw logging data and passes the final output message
        // an output Sink and child Dispatches.
        template <typename ... Args>
        void write(LogLevel level, const char *src_file, const char *src_func, int line, const char *fmt, Args const & ...args) {
            if(static_cast<int>(this->log_level & level) != 0) {
                Metadata meta = {
                    level,
                    level_to_str(level),
                    src_file,
                    src_func,
                    line
                };

                if(this->filter_func == nullptr || this->filter_func(meta)) {
                    char buf_out[buf_size];
                    char buf_msg[buf_size];

                    // Prevent access to uninitialized memory in case the
                    // format function does not fill the buffer.
                    buf_out[0] = '\0';

                    // Process the printf style input from the logging macros
                    snprintf(buf_msg, buf_size, fmt, args...);

                    // Pass the formatted log message to the format function
                    // for further adjustments with the metadata.
                    this->format_func(meta, buf_msg, buf_out);

                    if(output_sink != nullptr) {
                        // Locks the mutex for thread safe access to the
                        // output sink.
                        std::lock_guard<std::mutex> lock(mtx);
                        this->output_sink->write_ext(meta, buf_out);
                    }

                    // Pass the log message to all children Dispatch.
                    for(auto const& val: this->chain) {
                        val->write(level, src_file, src_func, line, buf_msg);
                    }
                }
            }
        }

        static std::unique_ptr<Dispatch> &get_logger() {
            if(!global_dispatch)
                throw std::logic_error("Logger has been accessed without being initialized");

            return global_dispatch;
        }
    private:
        // Prevent direct construction to enforce the usage of the
        // DisbatchBuilder.
        Dispatch();

        bool is_def_format = true;
        bool is_def_level = true;

        FnFilter filter_func;
        FnFormat format_func;
        LogLevel log_level;
        std::unique_ptr<Sink> output_sink;
        std::vector<std::unique_ptr<Dispatch>> chain;

        // Holds the global root Dispatch being used by the logging macros.
        static std::unique_ptr<Dispatch> global_dispatch;

        // Used for locking prior to potentional thread unsafe methods.
        static std::mutex mtx;
    };
}

#endif //DISPATCH_H
