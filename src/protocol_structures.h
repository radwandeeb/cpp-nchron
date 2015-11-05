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

#pragma pack(1)
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

typedef struct
{
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
    uint16_t offset;
} msgTimStd_t;

typedef struct
{
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
    uint16_t offset;
} msgTimDst_t;

}

#endif /* defined(__nchron__protocol_structures__) */