#include "dispatch.h"

namespace Kern {
    std::unique_ptr<Dispatch> Dispatch::global_dispatch = nullptr;
    std::mutex Dispatch::mtx;

    Dispatch::Dispatch() {
        this->output_sink = std::make_unique<StdoutSink>();
        this->log_level = LogLevel::All;
        this->filter_func = nullptr;
        this->format_func = [](auto meta, auto msg, auto buf) {
            snprintf(buf, 256, "[%s] %s", meta.level_str, msg);
        };
    }

    Dispatch::~Dispatch() {
        this->dchain.clear();
        this->output_sink = nullptr;
    }
}
