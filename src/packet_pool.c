#include "packet_pool.h"
#include <stdlib.h>
#include <stdatomic.h>

struct PacketPool {
    Packet packets[PACKET_POOL_SIZE];
    Packet* free_stack[PACKET_POOL_SIZE];
    size_t stack_top;
    atomic_flag lock; 
};

PacketPool *packet_pool_create(void){
    PacketPool *pool = calloc(1, sizeof(PacketPool));
    if (!pool) return NULL;
    
    for(size_t i = 0; i < PACKET_POOL_SIZE; i++){
        pool->free_stack[i] = &pool->packets[i];
    }
    pool->stack_top = PACKET_POOL_SIZE;
    
    atomic_flag_clear(&pool->lock);

    return pool;
}

void packet_pool_destroy(PacketPool *pool){
    if (!pool) return;
    free(pool);
}

Packet *packet_pool_acquire(PacketPool *pool){
    if(!pool) return NULL;

    while (atomic_flag_test_and_set(&pool->lock)) {
        // Spinlock
    }

    Packet *packet = NULL;
    if(pool->stack_top > 0){
        pool->stack_top--;
        packet = pool->free_stack[pool->stack_top];
    }

    atomic_flag_clear(&pool->lock);

    return packet;
}

void packet_pool_release(PacketPool *pool, Packet *packet){
    if(!pool || !packet) return;

    while (atomic_flag_test_and_set(&pool->lock)) {
        // Spinlock
    }

    if(pool->stack_top < PACKET_POOL_SIZE){
        pool->free_stack[pool->stack_top] = packet;   
        pool->stack_top++;
    }

    atomic_flag_clear(&pool->lock);
}