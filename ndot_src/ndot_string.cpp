#include "protocol.h"
#include "ndot_string.h"

void setString84x7(protocol::msgFpdR12_t *d, std::string str)
{
    uint8_t i = 0;
    for(std::string::const_iterator it = str.begin(); it != str.end() && (i < 14); ++it)
    {
        for(uint8_t col = 0; col < 6; ++col)
        {
            d->reg[col + (i * 6)] = font[*it][col];
        }
        i++;
    }
}

void setString112x16(protocol::msgFpdR16_t *d, std::string str)
{
    uint8_t i = 0;
    for(std::string::const_iterator it = str.begin(); it != str.end() && (i < 16); ++it)
    {
        for(uint8_t col = 0; col < 6; ++col)
        {
            d->reg[col + (i * 6)] = font[*it][col];
        }
        i++;
    }
}

void setStringHighLow112x16(protocol::msgFpdR16_t *d, std::string strh, std::string strl)
{
    uint8_t i = 0;
    for(std::string::const_iterator it = strh.begin(); it != strh.end() && (i < 16); ++it)
    {
        for(uint8_t col = 0; col < 6; ++col)
        {
            d->reg[col + (i * 6)] = font[*it][col];
        }
        i++;
    }

    i = 0;
    for(std::string::const_iterator it = strl.begin(); it != strl.end() && (i < 16); ++it)
    {
        for(uint8_t col = 0; col < 6; ++col)
        {
            d->reg[col + (i * 6)] += font[*it][col] << 8;
        }
        i++;
    }
}
