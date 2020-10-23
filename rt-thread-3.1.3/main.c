/*
* Copyright (c) 2006-2022, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2020-10-22     thread-liu   first version
*/

#include <stdio.h>
#include <rtthread.h>
#include <pthread.h>
#include <unistd.h>
#include "board.h"

#if defined(__EMSCRIPTEN__) 
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

int rtt_main(void)
{
    rt_kprintf("hello, rt-thread!\n");

    return 0;
}

int main(void)
{
    extern int entry(void);
    extern void *thread_serial(void *arg);

    int err = 0;
    unsigned char rbuf[24];
    unsigned char ch = 0, i = 0;
    pthread_t thread;

    entry();

    err = pthread_create(&thread, NULL, thread_serial, NULL);
    if (err != 0)
        printf("can't create thread\n");
    emscripten_unwind_to_js_event_loop();

    return 0;
}
