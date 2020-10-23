/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-10-21     thread-liu        the first version
 */

#include <rtthread.h>
#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include "serial.h"

#if defined(__EMSCRIPTEN__) 
#include <emscripten.h>
#include <emscripten/html5.h>
#endif

/* ringbuffer */
#define rt_ringbuffer_space_len(rb) ((rb)->buffer_size - rt_ringbuffer_data_len(rb))

struct rt_ringbuffer
{
    rt_uint8_t *buffer_ptr;

    rt_uint16_t read_mirror : 1;
    rt_uint16_t read_index : 15;
    rt_uint16_t write_mirror : 1;
    rt_uint16_t write_index : 15;

    rt_int16_t buffer_size;
};

enum rt_ringbuffer_state
{
    RT_RINGBUFFER_EMPTY,
    RT_RINGBUFFER_FULL,
    /* half full is neither full nor empty */
    RT_RINGBUFFER_HALFFULL,
};

rt_inline enum rt_ringbuffer_state rt_ringbuffer_status(struct rt_ringbuffer *rb)
{
    if (rb->read_index == rb->write_index)
    {
        if (rb->read_mirror == rb->write_mirror)
            return RT_RINGBUFFER_EMPTY;
        else
            return RT_RINGBUFFER_FULL;
    }
    return RT_RINGBUFFER_HALFFULL;
}

/** 
 * get the size of data in rb 
 */
rt_size_t rt_ringbuffer_data_len(struct rt_ringbuffer *rb)
{
    switch (rt_ringbuffer_status(rb))
    {
    case RT_RINGBUFFER_EMPTY:
        return 0;
    case RT_RINGBUFFER_FULL:
        return rb->buffer_size;
    case RT_RINGBUFFER_HALFFULL:
    default:
        if (rb->write_index > rb->read_index)
            return rb->write_index - rb->read_index;
        else
            return rb->buffer_size - (rb->read_index - rb->write_index);
    };
}

void rt_ringbuffer_init(struct rt_ringbuffer *rb,
                        rt_uint8_t           *pool,
                        rt_int16_t            size)
{
    RT_ASSERT(rb != RT_NULL);
    RT_ASSERT(size > 0);

    /* initialize read and write index */
    rb->read_mirror = rb->read_index = 0;
    rb->write_mirror = rb->write_index = 0;

    /* set buffer pool and size */
    rb->buffer_ptr = pool;
    rb->buffer_size = RT_ALIGN_DOWN(size, RT_ALIGN_SIZE);
}

/**
 * put a character into ring buffer
 */
rt_size_t rt_ringbuffer_putchar(struct rt_ringbuffer *rb, const rt_uint8_t ch)
{
    RT_ASSERT(rb != RT_NULL);

    /* whether has enough space */
    if (!rt_ringbuffer_space_len(rb))
        return 0;

    rb->buffer_ptr[rb->write_index] = ch;

    /* flip mirror */
    if (rb->write_index == rb->buffer_size-1)
    {
        rb->write_mirror = ~rb->write_mirror;
        rb->write_index = 0;
    }
    else
    {
        rb->write_index++;
    }

    return 1;
}
/**
 * get a character from a ringbuffer
 */
rt_size_t rt_ringbuffer_getchar(struct rt_ringbuffer *rb, rt_uint8_t *ch)
{
    RT_ASSERT(rb != RT_NULL);

    /* ringbuffer is empty */
    if (!rt_ringbuffer_data_len(rb))
        return 0;

    /* put character */
    *ch = rb->buffer_ptr[rb->read_index];

    if (rb->read_index == rb->buffer_size-1)
    {
        rb->read_mirror = ~rb->read_mirror;
        rb->read_index = 0;
    }
    else
    {
        rb->read_index++;
    }

    return 1;
}

/* emscripten virtual serial */
const char *emscripten_result_to_string(EMSCRIPTEN_RESULT result) 
{
    if (result == EMSCRIPTEN_RESULT_SUCCESS) 
        return "EMSCRIPTEN_RESULT_SUCCESS";
    if (result == EMSCRIPTEN_RESULT_DEFERRED) 
        return "EMSCRIPTEN_RESULT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) 
        return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
    if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) 
        return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
    if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) 
        return "EMSCRIPTEN_RESULT_INVALID_TARGET";
    if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) 
        return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
    if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) 
        return "EMSCRIPTEN_RESULT_INVALID_PARAM";
    if (result == EMSCRIPTEN_RESULT_FAILED) 
        return "EMSCRIPTEN_RESULT_FAILED";
    if (result == EMSCRIPTEN_RESULT_NO_DATA) 
        return "EMSCRIPTEN_RESULT_NO_DATA";

    return "Unknown EMSCRIPTEN_RESULT!";
}

#define TEST_RESULT(x) if (ret != EMSCRIPTEN_RESULT_SUCCESS) printf("%s returned %s.\n", #x, emscripten_result_to_string(ret));

static inline const char *emscripten_event_type_to_string(int eventType) 
{
    const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "(invalid)" };
    ++eventType;
    if (eventType < 0) 
    {
        eventType = 0;
    }
    if (eventType >= sizeof(events)/sizeof(events[0]))
    { 
        eventType = sizeof(events)/sizeof(events[0])-1;
    }
    return events[eventType];
}

// The event handler functions can return 1 to suppress the event and disable the default action. That calls event.preventDefault();
// Returning 0 signals that the event was not consumed by the code, and will allow the event to pass on and bubble up normally.
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
{

    if (eventType != EMSCRIPTEN_EVENT_KEYPRESS)
        return 0;

    rt_kprintf("%s, key: \"%s\", code: \"%s\", location: %lu,%s%s%s%s repeat: %d, locale: \"%s\", char: \"%s\", charCode: %lu, keyCode: %lu, which: %lu\n",
        emscripten_event_type_to_string(eventType), e->key, e->code, e->location,
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "",
        e->repeat, e->locale, e->charValue, e->charCode, e->keyCode, e->which);

    return 0;
}

void *thread_serial(void *arg)
{
    EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);

    EM_ASM(noExitRuntime = true);

    return 0;
}

void rt_hw_console_output(const char *str)
{
    char a = '\r';

    printf(str);
    printf(a);
}

char rt_hw_console_getchar(void)
{
    int ch = -1;


    return ch;
}
