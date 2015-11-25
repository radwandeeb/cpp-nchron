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

const uint16_t msg_id_cfg_sav = 0x0101;
const uint16_t msg_id_cfg_msg = 0x0102;

const uint16_t msg_id_tim_utc = 0x0201;
const uint16_t msg_id_tim_std = 0x0202;
const uint16_t msg_id_tim_dst = 0x0203;
const uint16_t msg_id_tim_src = 0x0204;

const uint16_t msg_id_nix_map = 0x0801;
const uint16_t msg_id_nix_scr = 0x0802;
const uint16_t msg_id_nix_tst = 0x0803;
const uint16_t msg_id_nix_raw = 0x0804;

const uint16_t msg_id_fpd_typ = 0x0901;
const uint16_t msg_id_fpd_mod = 0x0902;
const uint16_t msg_id_fpd_tst = 0x0903;
const uint16_t msg_id_fpd_r12 = 0x0904;
const uint16_t msg_id_fpd_r16 = 0x0905;

template<typename T> inline uint16_t getMsgId(T t)
{
    return 0;
}
template<> inline uint16_t getMsgId<msgTimUtc_t>(msgTimUtc_t t)
{
    return msg_id_tim_utc;
}
template<> inline uint16_t getMsgId<msgTimStd_t>(msgTimStd_t t)
{
    return msg_id_tim_std;
}
template<> inline uint16_t getMsgId<msgTimDst_t>(msgTimDst_t t)
{
    return msg_id_tim_dst;
}
template<> inline uint16_t getMsgId<msgFpdTyp_t>(msgFpdTyp_t t)
{
    return msg_id_fpd_typ;
}
template<> inline uint16_t getMsgId<msgFpdMod_t>(msgFpdMod_t t)
{
    return msg_id_fpd_mod;
}
template<> inline uint16_t getMsgId<msgFpdTst_t>(msgFpdTst_t t)
{
    return msg_id_fpd_tst;
}
template<> inline uint16_t getMsgId<msgFpdR12_t>(msgFpdR12_t t)
{
    return msg_id_fpd_r12;
}
template<> inline uint16_t getMsgId<msgFpdR16_t>(msgFpdR16_t t)
{
    return msg_id_fpd_r16;
}

template<typename T>
bool Protocol::sendMsg(T msg) const
{
    uint16_t msg_id = getMsgId(msg);

    packet_t packet;
    populateHeader(&packet, msg_id, sizeof(T));
    memcpy(packet.payload, &msg, sizeof(T));
    packet.checksum = calculateCheckSum(&packet);
    sendMessage(&packet);

    if(!getPacket(&packet, 1000))
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

template<typename T>
bool Protocol::sendMsgPoll(T *msg) const
{
    uint16_t msg_id = getMsgId(*msg);

    packet_t packet;
    populateHeader(&packet, msg_id, 0);
    packet.checksum = calculateCheckSum(&packet);
    sendMessage(&packet);

    if(!getPacket(&packet, 1000))
    {
        std::cout << "TIMEOUT!!!!" <<  std::endl;
        return false;
    }

    if (calculateCheckSum(&packet) != packet.checksum)
    {
        std::cout << "CHECKSUM ERROR!!!!" << std::endl;
        return false;
    }

    memcpy(msg, packet.payload, sizeof(T));
    return true;
}

template bool Protocol::sendMsg<msgTimUtc_t>(msgTimUtc_t msg) const;
template bool Protocol::sendMsg<msgTimStd_t>(msgTimStd_t msg) const;
template bool Protocol::sendMsg<msgTimDst_t>(msgTimDst_t msg) const;
template bool Protocol::sendMsg<msgFpdTyp_t>(msgFpdTyp_t msg) const;
template bool Protocol::sendMsg<msgFpdMod_t>(msgFpdMod_t msg) const;
template bool Protocol::sendMsg<msgFpdTst_t>(msgFpdTst_t msg) const;
template bool Protocol::sendMsg<msgFpdR12_t>(msgFpdR12_t msg) const;
template bool Protocol::sendMsg<msgFpdR16_t>(msgFpdR16_t msg) const;

template bool Protocol::sendMsgPoll<msgTimUtc_t>(msgTimUtc_t *msg) const;
template bool Protocol::sendMsgPoll<msgTimStd_t>(msgTimStd_t *msg) const;
template bool Protocol::sendMsgPoll<msgTimDst_t>(msgTimDst_t *msg) const;
template bool Protocol::sendMsgPoll<msgFpdTyp_t>(msgFpdTyp_t *msg) const;
template bool Protocol::sendMsgPoll<msgFpdMod_t>(msgFpdMod_t *msg) const;