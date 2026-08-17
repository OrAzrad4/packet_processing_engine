#include "ring_buffer.h"
#include <stdlib.h>
#include <stdatomic.h>



struct RingBuffer {
    PacketDescriptor *buffer;
    size_t capacity;    // Power of 2
    atomic_size_t read_index;
    atomic_size_t write_index;
};

RingBuffer *ring_buffer_create(size_t capacity){

    if (capacity == 0 || (capacity & (capacity - 1)) != 0) {
        return NULL; // Capacity must be a power of 2
    }
    RingBuffer *rb = malloc(sizeof(RingBuffer));
    if (!rb) {
        return NULL; // Allocation failed
    }
    rb->buffer = malloc(capacity * sizeof(PacketDescriptor));
    if (!rb->buffer) {
        free(rb);
        return NULL; // Allocation failed
    }
    rb->capacity = capacity;
    atomic_init(&rb->read_index, 0);
    atomic_init(&rb->write_index, 0);
    return rb;
}

void ring_buffer_destroy(RingBuffer *rb){
    if (!rb) {
        return;
    }
    free(rb->buffer);
    free(rb);
}

bool ring_buffer_push(RingBuffer *rb, const PacketDescriptor *descriptor){

    size_t write_idx = atomic_load_explicit(&rb->write_index,memory_order_relaxed);
    size_t read_idx = atomic_load_explicit(&rb->read_index,memory_order_acquire);

    if (((write_idx + 1) & (rb->capacity - 1)) == read_idx) {
        return false; // Buffer is full
    }
    rb->buffer[write_idx] = *descriptor;
    atomic_store_explicit(&rb->write_index,(write_idx+1) & (rb->capacity-1), memory_order_release);
    return true;
}

bool ring_buffer_pop(RingBuffer *rb, PacketDescriptor *descriptor){
    size_t read_idx = atomic_load_explicit(&rb->read_index,memory_order_relaxed);
    size_t write_idx = atomic_load_explicit(&rb->write_index,memory_order_acquire);

    if (read_idx == write_idx) {
        return false; // Buffer is empty
    }
    *descriptor = rb->buffer[read_idx];
    atomic_store_explicit(&rb->read_index,(read_idx +1 ) & (rb->capacity -1),memory_order_release);
    return true;
}

