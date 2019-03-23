#include "kern.h"

int main(int argc, char **argv) {
    kern::DispatchBuilder()
        .apply();

    debug("foo");
    info("bar");
    warning("baz");

    return 0;
}
