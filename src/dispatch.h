#ifndef DISPATCH_H
#define DISPATCH_H

#include <vector>
#include <memory>
#include <functional>
#include "sink.h"

namespace Kern {
    const size_t buf_size = 256;

    typedef std::function<void (Metadata &, const char *, char *)> FnFormat;
    typedef std::function<bool (Metadata &)> FnFilter;

    enum class LogLevel : char;

    class DispatchBuilder;

    class Dispatch {
        friend class DispatchBuilder;
    public:
        ~Dispatch();
        Dispatch(const Dispatch &) = delete;
        Dispatch& operator=(const Dispatch &) = delete;

        template <typename ... Args>
        void write(LogLevel level, const char *src, const char *fn, int line, const char *fmt, Args const & ...args) noexcept {
            if(static_cast<int>(this->log_level & level) != 0) {
                Metadata meta = {
                    .level = level,
                    .level_str = level_to_str(level),
                    .file = src,
                    .function = fn,
                    .line = line
                };

                if(this->filter_func == nullptr || this->filter_func(meta)) {
                    char buf_out[buf_size];
                    char buf_msg[buf_size];

                    // prevent access to uninitialized memory in case the
                    // format function does not fill the buffer
                    buf_out[0] = '\0';

                    snprintf(buf_msg, buf_size, fmt, args...);

                    this->format_func(meta, buf_msg, buf_out);

                    if(output_sink != nullptr)
                        this->output_sink->write_ext(meta, buf_out);

                    for(auto const& val: this->dchain) {
                        val->write(level, src, fn, line, buf_msg);
                    }
                }
            }
        }

        static std::unique_ptr<Dispatch> &get_logger() {
            return global_dispatch;
        }
    private:
        Dispatch();

        bool is_def_format = true;
        bool is_def_level = true;
        FnFilter filter_func;
        FnFormat format_func;
        LogLevel log_level;
        std::unique_ptr<Sink> output_sink;
        std::vector<std::unique_ptr<Dispatch>> dchain;

        static std::unique_ptr<Dispatch> global_dispatch;
    };
}

#endif //DISPATCH_H
