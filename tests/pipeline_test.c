#include "packet_pool.h"
#include "ring_buffer.h"
#include <assert.h>

int main()
{
    PacketPool *pool = packet_pool_create();
    RingBuffer *rb = ring_buffer_create(8);

    Packet *packet = packet_pool_acquire(pool);

    packet->id = 42;
    packet->length = 5;

    PacketDescriptor descriptor = {
        .packet = packet,
        .length = packet->length,
        .id = packet->id
    };

    bool pushed = ring_buffer_push(rb, &descriptor);

    PacketDescriptor received_descriptor;
    bool popped = ring_buffer_pop(rb, &received_descriptor);
    Packet* received_packet = received_descriptor.packet;

    assert(pushed);
    assert(popped);
    assert(received_descriptor.id == 42);
    assert(received_descriptor.length == 5);
    assert(received_packet == packet);
    packet_pool_release(pool, received_packet);


    Packet *packet2 = packet_pool_acquire(pool);

    assert(packet2 != NULL);
    assert(packet2 == packet);
    packet_pool_release(pool, packet2);
    packet_pool_destroy(pool);
    ring_buffer_destroy(rb);


    return 0;
}