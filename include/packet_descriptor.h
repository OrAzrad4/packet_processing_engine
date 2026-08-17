#ifndef PACKET_DESCRIPTOR_H
#define PACKET_DESCRIPTOR_H

#include <stdint.h>
#include "packet.h"

typedef struct {
    Packet *packet;
    uint16_t length;
    uint32_t id;
} PacketDescriptor;

#endif