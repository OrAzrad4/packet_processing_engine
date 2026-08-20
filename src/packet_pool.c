
#include "packet_pool.h"
#include <stdlib.h>


 struct PacketPool {
    Packet packets[PACKET_POOL_SIZE];
    Packet* free_stack[PACKET_POOL_SIZE];
    size_t stack_top;
} ;




PacketPool *packet_pool_create(void){
    PacketPool *pool = calloc(1, sizeof(PacketPool));
    if (!pool) {
        return NULL; // Allocation failed
    }
    for(size_t i=0; i < PACKET_POOL_SIZE; i++){
        pool->free_stack[i] = &pool->packets[i];
    }
    pool->stack_top = PACKET_POOL_SIZE;

    return pool;
}


void packet_pool_destroy(PacketPool *pool){
    if (!pool) {
        return;
    }
    free(pool);
}

Packet *packet_pool_acquire(PacketPool *pool){
    if(!pool || pool->stack_top == 0){
        return NULL;
    }
    pool->stack_top--;
    return pool->free_stack[pool->stack_top];
}

void packet_pool_release(PacketPool *pool, Packet *packet){
    if(!pool || !packet || pool->stack_top >= PACKET_POOL_SIZE){
        return;
    }
    pool->free_stack[pool->stack_top] = packet;   
    pool->stack_top++;
}