#ifndef PACKET_POOL_H
#define PACKET_POOL_H

#include <stdbool.h>
#include "packet.h"

#define PACKET_POOL_SIZE 1024


typedef struct PacketPool PacketPool;

PacketPool *packet_pool_create(void);

void packet_pool_destroy(PacketPool *pool);

Packet *packet_pool_acquire(PacketPool *pool);

void packet_pool_release(PacketPool *pool, Packet *packet);

#endif