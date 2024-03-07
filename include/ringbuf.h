/*
 * SPDX-FileCopyrightText: 2024 Dmitrii Lebed <lebed.dmitry@gmail.com>
 * SPDX-License-Identifier: BSD-2-Clause 
 */

#ifndef RINGBUF_H
#define RINGBUF_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define MIN(x, y)   (((x) < (y)) ? (x) : (y))
#define MAX(x, y)   (((x) > (y)) ? (x) : (y))

struct ringbuf_common {
    /* runtime variables */
    size_t                  rp;
    size_t                  wp;
    size_t                  size;       /* number of elements currently stored in the buffer */
    /* configuration */
    size_t                  capacity;   /* capacity in number of elements (not bytes) */
};

struct ringbuf_u8 {
    struct ringbuf_common   common;
    uint8_t                 *buf;
};

static inline void ringbuf_common_init(struct ringbuf_common *cb, size_t capacity)
{
    cb->rp = 0;
    cb->wp = 0;
    cb->size = 0;
    cb->capacity = capacity;
}

static inline size_t ringbuf_common_capacity(const struct ringbuf_common *cb)
{
    return cb->capacity;
}

static inline size_t ringbuf_common_size(const struct ringbuf_common *cb)
{
    return cb->size;
}

static inline void ringbuf_common_push_single(struct ringbuf_common *cb)
{
    cb->wp++;
    cb->size++;

    if (cb->wp == cb->capacity)
        cb->wp = 0;
}

static inline void ringbuf_common_pop_single(struct ringbuf_common *cb)
{
    cb->rp++;
    cb->size--;

    if (cb->rp == cb->capacity)
        cb->rp = 0;
}

static inline size_t ringbuf_common_push_contiguous(struct ringbuf_common *cb, size_t size)
{
    size = MIN(size, cb->capacity - cb->size);
    size = MIN(size, cb->capacity - cb->wp);

    cb->size += size;
    cb->wp += size;

    if (cb->wp == cb->capacity)
        cb->wp = 0;

    return size;
}

static inline size_t ringbuf_common_pop_contiguous(struct ringbuf_common *cb, size_t size)
{
    size = MIN(size, cb->size);
    size = MIN(size, cb->capacity - cb->rp);

    cb->size -= size;
    cb->rp += size;

    if (cb->rp == cb->capacity)
        cb->rp = 0;

    return size;
}

static inline void ringbuf_u8_init(struct ringbuf_u8 *cb, uint8_t *buf, size_t size)
{
    ringbuf_common_init(&cb->common, size);
    cb->buf = buf;
}

static inline size_t ringbuf_u8_capacity(const struct ringbuf_u8 *cb)
{
    return ringbuf_common_capacity(&cb->common);
}

static inline size_t ringbuf_u8_size(const struct ringbuf_u8 *cb)
{
    return ringbuf_common_size(&cb->common);
}

static inline bool ringbuf_u8_is_empty(const struct ringbuf_u8 *cb)
{
    return ringbuf_u8_size(cb) == 0;
}

static inline bool ringbuf_u8_is_full(const struct ringbuf_u8 *cb)
{
    return ringbuf_u8_size(cb) == ringbuf_u8_capacity(cb);
}

static inline size_t ringbuf_u8_push_byte(struct ringbuf_u8 *cb, uint8_t byte)
{
    if (ringbuf_u8_is_full(cb))
        return 0;

    cb->buf[cb->common.wp] = byte;
    ringbuf_common_push_single(&cb->common);

    return sizeof(byte);
}

static inline size_t ringbuf_u8_pop_byte(struct ringbuf_u8 *cb, uint8_t *byte)
{
    if (ringbuf_u8_is_empty(cb))
        return 0;

    *byte = cb->buf[cb->common.rp];
    ringbuf_common_pop_single(&cb->common);

    return sizeof(*byte);
}

#ifdef __cplusplus
}
#endif

#endif //RINGBUF_H
