#include "ring_buffer.h"
#include <cassert>
#include <iostream>

int main()
{
    RingBuffer* rb = ring_buffer_create(4);
    assert(rb != nullptr);

    Packet packet = {};
    packet.id = 42;
    packet.length = 5;

    PacketDescriptor descriptor = {
        .packet = &packet,
        .length = packet.length,
        .id = packet.id
    };

    bool pushed = ring_buffer_push(rb, &descriptor);
    assert(pushed);

    PacketDescriptor result = {};

    bool popped = ring_buffer_pop(rb, &result);
    assert(popped);

    assert(result.id == 42);
    assert(result.length == 5);
    assert(result.packet == &packet);

    ring_buffer_destroy(rb);

    std::cout << "Test passed!\n";
}