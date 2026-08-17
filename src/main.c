#include "packet_pool.h"
#include "ring_buffer.h"

#include <pthread.h>
#include <stdio.h>
#include <unistd.h>

typedef struct {      // Pipeline context structure beacause we need to pass both the packet pool and the ring buffer to the thread function
    PacketPool *pool;
    RingBuffer *ring;
} PipelineContext;

void *producer_thread(void *arg){
    PipelineContext *ctx = arg;
    Packet* packet = packet_pool_acquire(ctx->pool);
        if (!packet) {
        return NULL;
    }

    packet->id = 1;
    packet->length = 5;

    PacketDescriptor descriptor = {
        .packet = packet,
        .length = packet->length,
        .id = packet->id
    };
    sleep(1);
    ring_buffer_push(ctx->ring, &descriptor);

    return NULL;
}


void *consumer_thread(void *arg)
{
    PipelineContext *ctx = arg;

    PacketDescriptor descriptor;
    while(!ring_buffer_pop(ctx->ring,&descriptor)){
        // Polling
    }
        printf("Received packet %u\n", descriptor.id);

        packet_pool_release(ctx->pool, descriptor.packet);
    

    return NULL;
}

int main(void)
{
    PacketPool *pool = packet_pool_create();
    RingBuffer *ring = ring_buffer_create(8);

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
