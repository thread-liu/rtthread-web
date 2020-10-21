/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-10-21     thread-liu        the first version
 */

#include <pthread.h>
#include <unistd.h>
#include <stdio.h>
#include <emscripten.h>
#include <string.h>
#include <emscripten/html5.h>
#include "serial.h"

static unsigned char rx_buffer[MAX_BUFFER_SIZE];
static unsigned char tx_buffer[MAX_BUFFER_SIZE];

struct rthw_serial
{
    struct rt_serial serial;
};
static struct rthw_serial dev_serial;

const char *emscripten_result_to_string(EMSCRIPTEN_RESULT result) {
  if (result == EMSCRIPTEN_RESULT_SUCCESS) return "EMSCRIPTEN_RESULT_SUCCESS";
  if (result == EMSCRIPTEN_RESULT_DEFERRED) return "EMSCRIPTEN_RESULT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_NOT_SUPPORTED) return "EMSCRIPTEN_RESULT_NOT_SUPPORTED";
  if (result == EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED) return "EMSCRIPTEN_RESULT_FAILED_NOT_DEFERRED";
  if (result == EMSCRIPTEN_RESULT_INVALID_TARGET) return "EMSCRIPTEN_RESULT_INVALID_TARGET";
  if (result == EMSCRIPTEN_RESULT_UNKNOWN_TARGET) return "EMSCRIPTEN_RESULT_UNKNOWN_TARGET";
  if (result == EMSCRIPTEN_RESULT_INVALID_PARAM) return "EMSCRIPTEN_RESULT_INVALID_PARAM";
  if (result == EMSCRIPTEN_RESULT_FAILED) return "EMSCRIPTEN_RESULT_FAILED";
  if (result == EMSCRIPTEN_RESULT_NO_DATA) return "EMSCRIPTEN_RESULT_NO_DATA";
  return "Unknown EMSCRIPTEN_RESULT!";
}

#define TEST_RESULT(x) if (ret != EMSCRIPTEN_RESULT_SUCCESS) printf("%s returned %s.\n", #x, emscripten_result_to_string(ret));

static inline const char *emscripten_event_type_to_string(int eventType) {
  const char *events[] = { "(invalid)", "(none)", "keypress", "keydown", "keyup", "(invalid)" };
  ++eventType;
  if (eventType < 0) eventType = 0;
  if (eventType >= sizeof(events)/sizeof(events[0])) eventType = sizeof(events)/sizeof(events[0])-1;
  return events[eventType];
}

// The event handler functions can return 1 to suppress the event and disable the default action. That calls event.preventDefault();
// Returning 0 signals that the event was not consumed by the code, and will allow the event to pass on and bubble up normally.
EM_BOOL key_callback(int eventType, const EmscriptenKeyboardEvent *e, void *userData)
{

    unsigned short rx_size = 0, i = 0;
    unsigned short count, size, offset;
    unsigned char *buf = NULL;
    
    // if (eventType != EMSCRIPTEN_EVENT_KEYPRESS)
    //     return 0;

    // buf    = dev_serial.serial.rbuf; 
    // count  = dev_serial.serial.rbuf_count;
    // size   = dev_serial.serial.rbuf_size;
    // offset = dev_serial.serial.rbuf_start + count;
    printf("%s, key: \"%s\", code: \"%s\", location: %lu,%s%s%s%s repeat: %d, locale: \"%s\", char: \"%s\", charCode: %lu, keyCode: %lu, which: %lu\n",
        emscripten_event_type_to_string(eventType), e->key, e->code, e->location, 
        e->ctrlKey ? " CTRL" : "", e->shiftKey ? " SHIFT" : "", e->altKey ? " ALT" : "", e->metaKey ? " META" : "", 
        e->repeat, e->locale, e->charValue, e->charCode, e->keyCode, e->which);
    // if (count < size)
    // {
    //     if (offset >= size)
    //     {
    //         offset -= size;
    //     }
        
    //     buf[offset++] = e->key;
    //     count++;
    // }

    // dev_serial.serial.rbuf_count = count;

    return 0;
}

// static unsigned char read(void *buffer, unsigned short size)
// {
//     unsigned short count, rbsize, offset, i;
//     unsigned char *buf     = NULL; 
//     unsigned char *pBuffer = NULL;
    
    
//     pBuffer = (unsigned char*)buffer;
//     count   = dev_serial.serial.rbuf_count;
//     buf     = dev_serial.serial.rbuf;
    
//     if (count == 0)
//     {
//         return 0;
//     }
    
//     if (count >= size)
//     {
//         count = size;
//     } 

//     offset = dev_serial.serial.rbuf_start;
//     rbsize = dev_serial.serial.rbuf_size;
 
//     for (i = 0; i < count; i++)
//     {
//         *pBuffer++ = buf[offset++];
//         if (offset > rbsize)
//         {
//            offset = 0;  
//         }
//     }

//     dev_serial.serial.rbuf_start  = offset;
//     dev_serial.serial.rbuf_count -= count;
    
//     return count;
// }

void *thread_main(void *arg)
{
    EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);

    EM_ASM(noExitRuntime = true);

    return 0;
}

int main()
{
    int err = 0;
    unsigned char rbuf[24];
    unsigned char ch = 0, i = 0;
    pthread_t thread;

    err = pthread_create(&thread, NULL, thread_main, NULL); 
    if (err != 0)
        printf("can't create thread: %s\n", strerror(err));
    emscripten_unwind_to_js_event_loop();

    EMSCRIPTEN_RESULT ret = emscripten_set_keypress_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);

    // ret = emscripten_set_keydown_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
    // TEST_RESULT(emscripten_set_keydown_callback);
    // ret = emscripten_set_keyup_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, 0, 1, key_callback);
    // TEST_RESULT(emscripten_set_keyup_callback);

    // ch = read(rbuf, 24);
    // if (ch != 0)
    // {
    //     for (i = 0; i < ch; i++)
    //     {
    //         printf("%c", rbuf[0]);
    //     }
    // }
    /* For the events to function, one must either call emscripten_set_main_loop or enable Module.noExitRuntime by some other means.
        Otherwise the application will exit after leaving main(), and the atexit handlers will clean up all event hooks (by design). */

    return 0;
}
