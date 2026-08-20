#include "packet_pool.h"
#include "ring_buffer.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

#define NUM_PACKETS 10000

typedef struct {      // Pipeline context structure beacause we need to pass both the packet pool and the ring buffer to the thread function
    PacketPool *pool;
    RingBuffer *ring;
} PipelineContext;

void *producer_thread(void *arg){
    PipelineContext *ctx = arg;
    size_t dropped_packets = 0;

    for (size_t i = 1; i <= NUM_PACKETS; ) { 
        Packet* packet = NULL;
        
        while (!(packet = packet_pool_acquire(ctx->pool))) {
        }

        packet->id = i;
        packet->length = 64; 

        PacketDescriptor descriptor = {
            .packet = packet,
            .length = packet->length,
            .id = packet->id
        };

        if (!ring_buffer_push(ctx->ring, &descriptor)) {
            packet_pool_release(ctx->pool, packet);
            dropped_packets++;
        } else {
            i++; 
        }
    }

    printf("[Producer] Finished sending %d packets. Dropped: %zu\n", NUM_PACKETS, dropped_packets);
    return NULL;
}
void *consumer_thread(void *arg) {
    PipelineContext *ctx = arg;
    PacketDescriptor descriptor;

    for (size_t i = 1; i <= NUM_PACKETS; i++) {

        while (!ring_buffer_pop(ctx->ring, &descriptor)) {
            // Polling 
        }

        if (descriptor.id % 2000 == 0 || descriptor.id == NUM_PACKETS) {
            printf("[Consumer] Processed packet %u\n", descriptor.id);
        }

        packet_pool_release(ctx->pool, descriptor.packet);
    }

    printf("[Consumer] Finished processing %d packets.\n", NUM_PACKETS);
    return NULL;
}
int main(void)
{
    PacketPool *pool = packet_pool_create();
    RingBuffer *ring = ring_buffer_create(1024);

    PipelineContext ctx = {
        .pool = pool,
        .ring = ring
    };

    pthread_t producer;
    pthread_t consumer;

    pthread_create(&producer, NULL, producer_thread, &ctx);
    pthread_create(&consumer, NULL, consumer_thread, &ctx);

    pthread_join(producer, NULL);
    pthread_join(consumer, NULL);

    ring_buffer_destroy(ring);
    packet_pool_destroy(pool);

    return 0;
}
