#ifndef DISPATCH_H
#define DISPATCH_H

#include <vector>
#include <memory>
#include <functional>
#include "sink.h"

namespace Kern {
    typedef std::function<void (Metadata &, const char *, char *)> FnFormat;

    enum class LogLevel : char;

    class DispatchBuilder;

    class Dispatch {
        friend class DispatchBuilder;
    public:
        ~Dispatch();
        Dispatch(const Dispatch &) = delete;
        Dispatch& operator=(const Dispatch &) = delete;

        void write(LogLevel, const char *, const char *, int, const char *);

        static std::unique_ptr<Dispatch> &get_logger() {
            return global_dispatch;
        }
    private:
        Dispatch();

        bool is_def_format = true;
        bool is_def_level = true;
        char buf[265];
        FnFormat format_func;
        LogLevel log_level;
        std::unique_ptr<Sink> output_sink;
        std::vector<std::unique_ptr<Dispatch>> dchain;

        static std::unique_ptr<Dispatch> global_dispatch;
    };
}

#endif //DISPATCH_H
