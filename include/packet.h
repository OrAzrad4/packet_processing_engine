#ifndef PACKET_H
#define PACKET_H

#include <stdint.h>

#define MAX_PACKET_SIZE 1500

typedef struct {
    uint32_t id;
    uint16_t length;
    uint8_t data[MAX_PACKET_SIZE];
} Packet;

#endif