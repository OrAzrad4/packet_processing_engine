#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <stddef.h>
#include <stdbool.h>
#include "packet_descriptor.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct RingBuffer RingBuffer;

RingBuffer *ring_buffer_create(size_t capacity);
void ring_buffer_destroy(RingBuffer *rb);

bool ring_buffer_push(RingBuffer *rb, const PacketDescriptor *descriptor);
bool ring_buffer_pop(RingBuffer *rb, PacketDescriptor *descriptor);

bool ring_buffer_is_empty(const RingBuffer *rb);
bool ring_buffer_is_full(const RingBuffer *rb);

#ifdef __cplusplus
}
#endif

#endif