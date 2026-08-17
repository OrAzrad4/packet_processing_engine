#include "ring_buffer.h"
#include <stdlib.h>



struct RingBuffer {
    PacketDescriptor *buffer;
    size_t capacity;    // Power of 2
    size_t read_index;
    size_t write_index;
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
    rb->read_index = 0;
    rb->write_index = 0;
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
    if (((rb->write_index + 1) & (rb->capacity - 1)) == rb->read_index) {
        return false; // Buffer is full
    }
    rb->buffer[rb->write_index] = *descriptor;
    rb->write_index = (rb->write_index + 1) & (rb->capacity - 1);
    return true;
}

bool ring_buffer_pop(RingBuffer *rb, PacketDescriptor *descriptor){
    if (rb->read_index == rb->write_index) {
        return false; // Buffer is empty
    }
    *descriptor = rb->buffer[rb->read_index];
    rb->read_index = (rb->read_index + 1) & (rb->capacity - 1);
    return true;
}

