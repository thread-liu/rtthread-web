#include <stdlib.h>
#include <stdio.h>
#include <rtthread.h>

# define EMSCRIPTEN_KEEPALIVE

/* === RT-Thread Required Functions === */

rt_uint8_t *rt_hw_stack_init(void *tentry, void *parameter,
    rt_uint8_t *stack_addr, void *texit) {
    (void)tentry;
    (void)parameter;
    (void)texit;
    *((rt_uint32_t *)stack_addr) = 0;
    return stack_addr;
}

rt_base_t rt_hw_interrupt_disable(void) {
    rt_base_t original_level;

    original_level = 1;

    return original_level;
}

void rt_hw_interrupt_enable(rt_base_t level) {
    (void)level;
}

void rt_hw_context_switch(rt_ubase_t from, rt_ubase_t to) {
    // printf("rt_hw_context_switch %p %p\n", from, to);
}

void rt_hw_context_switch_to(rt_ubase_t to) {
    // printf("rt_hw_context_switch_to %p\n", to);
}

void rt_hw_context_switch_interrupt(rt_ubase_t from, rt_ubase_t to) {
    // printf("rt_hw_context_switch_interrupt %p %p\n", from, to);
}

// void rt_hw_console_output(const char *str) {
//     printf(str);
// }

EMSCRIPTEN_KEEPALIVE
void wasm_tick_increase(void) {
    struct rt_thread *thread;

    rt_interrupt_enter();
    rt_tick_increase();
    rt_interrupt_leave();

    thread = rt_thread_self();
    ((void (*)(void *))thread->entry)(thread->parameter);
}