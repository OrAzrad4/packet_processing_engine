
#include "packet_pool.h"
#include <stdlib.h>


 struct PacketPool {
    Packet packets[PACKET_POOL_SIZE];
    bool in_use[PACKET_POOL_SIZE];
} ;

PacketPool *packet_pool_create(void){
    PacketPool *pool = calloc(1, sizeof(PacketPool));
    if (!pool) {
        return NULL; // Allocation failed
    }
    return pool;
}


void packet_pool_destroy(PacketPool *pool){
    if (!pool) {
        return;
    }
    free(pool);
}

Packet *packet_pool_acquire(PacketPool *pool){
    if(!pool){
        return NULL;
    }
    for(size_t i=0;i<PACKET_POOL_SIZE;i++){
        if(pool->in_use[i] == false){
            pool->in_use[i] = true;
            return &pool->packets[i];
        }
    }
    return NULL;
}

void packet_pool_release(PacketPool *pool, Packet *packet){
    if(!pool || !packet){
        return;
    }
    
    if(packet >= pool->packets && packet < pool->packets + PACKET_POOL_SIZE ){
        size_t index = packet - pool->packets;
        pool->in_use[index] = false;
    }
}