//
//  protocol.cpp
//  nchron
//
//  Created by Janis on 26.05.15.
//  Copyright (c) 2015 Janis. All rights reserved.
//

#include "protocol.h"
#include "protocol_structures.h"

#include <iostream>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

using namespace protocol;

bool Protocol::sendMsgTimUtc(msgTimUtc_t msg) const
{
    packet_t packet;
    populateHeader(&packet, msg_id_tim_utc, sizeof(msgTimUtc_t));
    memcpy(packet.payload, &msg, sizeof(msgTimUtc_t));

    packet.checksum = calculateCheckSum(&packet);
    sendMessage(&packet);

    if(!getPacket(&packet, 20))
    {
        std::cout << "TIMEOUT!!!!" <<  std::endl;
        return false;
    }

    if (calculateCheckSum(&packet) != packet.checksum)
    {
        std::cout << "CHECKSUM ERROR!!!!" << std::endl;
        return false;
    }

    return true;
}

bool Protocol::sendMsgTimUtcPoll(msgTimUtc_t *msg) const
{
    packet_t packet;
    populateHeader(&packet, msg_id_tim_utc, 0);
    packet.checksum = calculateCheckSum(&packet);
    sendMessage(&packet);

    if(!getPacket(&packet, 20))
    {
        std::cout << "TIMEOUT!!!!" <<  std::endl;
        return false;
    }

    if (calculateCheckSum(&packet) != packet.checksum)
    {
        std::cout << "CHECKSUM ERROR!!!!" << std::endl;
        return false;
    }

    memcpy(&msg, packet.payload, sizeof(msgTimUtc_t));
    return true;
}