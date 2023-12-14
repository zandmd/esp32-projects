#include <assert.h>
#include <zandmd/drivers/rtos_init.hpp>

using namespace zandmd::drivers;

rtos_init *rtos_init::head = nullptr;
rtos_init **rtos_init::tail = &head;

rtos_init::~rtos_init() noexcept {
    assert(head == nullptr);
}

void rtos_init::post_init() noexcept {
    while (head) {
        rtos_init *it = head;
        head = it->next;
        it->post_rtos_init();
    }
    tail = &head;
}

rtos_init::rtos_init() noexcept : next(nullptr) {
    *tail = this;
    tail = &next;
}
