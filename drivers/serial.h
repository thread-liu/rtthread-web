/*
 * Copyright (c) 2006-2022, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author            Notes
 * 2020-10-21     thread-liu        the first version
 */

#ifndef __SERIAL_H__
#define __SERIAL_H__

#define MAX_BUFFER_SIZE 256

struct rt_serial
{
    unsigned char *rbuf; 
    unsigned char *tbuf;          
    volatile unsigned short  rbuf_size;      
    volatile unsigned short  tbuf_size;
    volatile unsigned short  rbuf_start;
    volatile unsigned short  rbuf_count;
    volatile unsigned short  tbuf_start;
    volatile unsigned short  tbuf_count;
};

#endif /* serial.h */ 
