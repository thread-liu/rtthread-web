/*
* Copyright (c) 2006-2022, RT-Thread Development Team
*
* SPDX-License-Identifier: Apache-2.0
*
* Change Logs:
* Date           Author       Notes
* 2020-10-22     thread-liu   first version
*/

#include <rthw.h>
#include <rtthread.h>

#include "board.h"

/**
 * This function will initialize beaglebone board
 */
static rt_uint8_t rtt_heap[CONFIG_HEAP_SIZE];

void rt_hw_board_init(void)
{
    /* initialize system heap */
#ifdef RT_USING_HEAP
        /* init heap */
    rt_system_heap_init((void *)&rtt_heap, (void *)&rtt_heap[CONFIG_HEAP_SIZE-1]);
#endif

    rt_components_board_init();
    // rt_console_set_device(RT_CONSOLE_DEVICE_NAME);

    // rt_thread_idle_sethook(idle_wfi);

#ifdef RT_USING_SMP
    /* install IPI handle */
    rt_hw_ipi_handler_install(RT_SCHEDULE_IPI, rt_scheduler_ipi_handler);
#endif
}
