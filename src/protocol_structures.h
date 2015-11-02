//
//  protocol_structures.h
//  nchron
//
//  Created by Janis on 26.05.15.
//  Copyright (c) 2015 Janis. All rights reserved.
//

#ifndef __nchron__protocol_structures__
#define __nchron__protocol_structures__


// define macro to pack structures correctly with both GCC and MSVC compilers
// #ifdef _MSC_VER // using MSVC
// #define PACK( __Declaration__ ) __pragma( pack(push, 1) ) __Declaration__ __pragma( pack(pop) )
// #else
// #define PACK( __Declaration__ ) __Declaration__ __attribute__((__packed__))
// #endif


namespace protocol
{

const uint16_t preamble = 0x62B5;

typedef enum
{
    DECODER_STATE_PREAMBLE_0 = 0,
    DECODER_STATE_PREAMBLE_1,
    DECODER_STATE_CLASS_ID,
    DECODER_STATE_MSG_ID,
    DECODER_STATE_LENGTH_0,
    DECODER_STATE_LENGTH_1,
    DECODER_STATE_PAYLOAD,
    DECODER_STATE_CHECKSUM_0,
    DECODER_STATE_CHECKSUM_1
} decoderState_t;

const uint16_t msg_id_cfg_sav = 0x0101;
const uint16_t msg_id_cfg_msg = 0x0102;

const uint16_t msg_id_tim_utc = 0x0201;
const uint16_t msg_id_tim_std = 0x0202;
const uint16_t msg_id_tim_dst = 0x0203;
const uint16_t msg_id_tim_src = 0x0204;

const uint16_t msg_id_nix_map = 0x0801;
const uint16_t msg_id_nix_scr = 0x0802;
const uint16_t msg_id_nix_tst = 0x0803;
const uint16_t msg_id_nix_man = 0x0804;

#define PAYLOAD_SIZE 0xFF

#pragma pack(1)
typedef struct
{
    uint16_t preamble;
    uint16_t messageId;
    uint16_t payloadLength;
    uint8_t payload[PAYLOAD_SIZE];
    uint16_t checksum;
} packet_t;

typedef struct
{
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
    uint16_t offset;
} msgTimTrs_t;

typedef struct
{
    int32_t nano;
    int16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t min;
    uint8_t sec;
    uint8_t valid;
} msgTimUtc_t;

}

#endif /* defined(__nchron__protocol_structures__) */