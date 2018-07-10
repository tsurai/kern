#include "dispatch.h"

namespace kern {
    std::unique_ptr<Dispatch> Dispatch::global_dispatch = nullptr;
    std::mutex Dispatch::mtx;

    // Constructs a valid Dispatch object. This Constructor can only be called
    // from the DispatchBuilder to initiate it's inner object with a valid
    // state.
    Dispatch::Dispatch() {
        this->output_sink = nullptr;
        this->log_level = LogLevel::All;
        this->filter_func = nullptr;
        this->format_func = [](auto meta, auto msg, auto buf) {
            snprintf(buf, 256, "[%s] %s", meta.level_str, msg);
        };
    }

    Dispatch::~Dispatch() {
        this->chain.clear();
        this->output_sink = nullptr;
    }
}
