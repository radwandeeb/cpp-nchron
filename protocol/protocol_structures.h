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
    int16_t offset;
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
} msgTimStd_t;

typedef struct
{
    int16_t offset;
    uint8_t hour;
    uint8_t dow;
    uint8_t week;
    uint8_t month;
} msgTimDst_t;

typedef struct
{
    uint32_t src;
    uint8_t mode;
} msgTimSrc_t;


typedef struct
{
    uint32_t mapping;
} msgNixMap_t;

typedef struct
{
    uint32_t interval;
    uint8_t type;
} msgNixScr_t;

typedef struct
{
    uint8_t test;
} msgNixTst_t;

typedef struct
{
    uint8_t reg[10];
} msgNixMan_t;


typedef struct
{
    uint8_t test;
} msgFpdTst_t;

typedef struct
{
    uint8_t type;
} msgFpdTyp_t;

typedef struct
{
    uint8_t mode;
} msgFpdMod_t;

typedef struct
{
    uint8_t reg[84];
} msgFpdR12_t;

typedef struct
{
    uint8_t reg[112];
} msgFpdR16_t;

}

#endif /* defined(__nchron__protocol_structures__) */