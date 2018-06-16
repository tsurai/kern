#include "kern.h"

namespace Kern {
    std::unique_ptr<Dispatch> Dispatch::global_dispatch = nullptr;

    Dispatch::Dispatch() {
        this->output_sink = std::make_unique<StdoutSink>();
        this->log_level = LogLevel::All;
        this->format_func = [](auto meta, auto msg, auto buf) { snprintf(buf, 256, "[%s] %s", meta.level, msg); };
    }

    Dispatch::~Dispatch() {
        this->dchain.clear();
        this->output_sink = nullptr;
    }

    void Dispatch::write(LogLevel level, const char *src_file, const char *fn_name, int line, const char *msg) {
        if(static_cast<int>(this->log_level & level) != 0) {
            Metadata meta = {
                .level = level_to_str(level),
                .file = src_file,
                .function = fn_name,
                .line = line
            };

            this->format_func(meta, msg, this->buf);

            if(output_sink != nullptr)
                this->output_sink->write(this->buf);

            for(auto const& val: this->dchain) {
                val->write(level, src_file, fn_name, line, msg);
            }
        }
    }
}
