//
//  protocol.h
//  nchron
//
//  Created by Janis on 26.05.15.
//  Copyright (c) 2015 Janis. All rights reserved.
//

#ifndef __nchron__protocol__
#define __nchron__protocol__

#include <stdio.h>
#include <string>
#include <iostream>

#include "stdint.h"
#include "protocol_structures.h"

namespace protocol
{

const uint16_t payloadSize = 0x5FF;

#pragma pack(1)
typedef struct
{
    uint16_t preamble;
    uint16_t messageId;
    uint16_t payloadLength;
    uint8_t payload[payloadSize];
    uint16_t checksum;
} packet_t;


class Protocol
{
public:
    Protocol(std::string port);
    ~Protocol();

    bool poll();

    template<typename T>
    bool sendMsg(T msg) const;

    template<typename T>
    bool sendMsgPoll(T *msg) const;

private:
    bool sendMessage(packet_t *packet) const;
    uint16_t calculateCheckSum(const packet_t *packet) const;
    bool checkPacket(const packet_t *packet) const;
    bool getPacket(packet_t *packet, uint32_t timeout) const;
    bool evaluatePacket(packet_t *packet);
    void populateHeader(packet_t *packet, const uint16_t messageId, const uint16_t payloadLength) const;

    std::string m_buffer;
    int m_portfd;

    uint16_t m_lastMsgId;
};

}


#endif /* defined(__nchron__protocol__) */
