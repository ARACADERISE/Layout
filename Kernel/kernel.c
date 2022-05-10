#include "util.h"
#include "gui.h"

__attribute__((section("kernel_entry"))) void _start(void) {

    init_GUI();

    while(1) {}
}
