#include "protocol.h"
#include "protocol_structures.h"

#include <iostream>
#include <iomanip>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <chrono>

using namespace protocol;

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

const uint16_t preamble = 0x62B5;


Protocol::Protocol(std::string port)
{
    int fd;
    fd = open(port.c_str(), O_RDWR | O_NOCTTY | O_NDELAY);
    if (fd == -1)
    {
        std::cerr << "ERROR... Unable to open port" << std::endl;
        abort();
    }
    else
    {
        std::cout << "Port opened" << std::endl;
        m_portfd = fd;
    }

    struct termios options;
    tcgetattr(fd, &options);
    cfsetispeed(&options, B115200);
    cfsetospeed(&options, B115200);
    options.c_cflag &= ~PARENB;
    options.c_cflag &= ~CSTOPB;
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;
    options.c_cflag |= (CLOCAL | CREAD);
    tcsetattr(fd, TCSANOW, &options);

    usleep(1000);
    tcflush(fd, TCIOFLUSH);
    usleep(1000);
}

Protocol::~Protocol()
{
    close(m_portfd);
}

void Protocol::populateHeader(packet_t *packet,
                              const uint16_t messageId,
                              const uint16_t payloadLength) const
{
    packet->preamble = preamble;
    packet->messageId = messageId;
    packet->payloadLength = payloadLength;
}

bool Protocol::getPacket(packet_t *packet, uint32_t timeout) const
{
    decoderState_t state = DECODER_STATE_PREAMBLE_0;
    uint8_t payloadCounter = 0;
    bool isWholePacket = false;

    auto ts = std::chrono::high_resolution_clock::now();

    while (!isWholePacket)
    {
        auto t = std::chrono::high_resolution_clock::now();
        auto d = t - ts;

        if (d > std::chrono::milliseconds(timeout))
        {
            std::cerr << "Timeout occurred after " << timeout << "ms" << std::endl;
            return false;
        }

        uint8_t c;
        while(read(m_portfd, &c, 1) > 0)
        {
            std::cout << std::hex << "RX: x" << (int)c << std::endl;
            switch(state)
            {
            case DECODER_STATE_PREAMBLE_0:
                //std::cout << "DECODER_STATE_SYNC_0" << std::endl;
                state = (c == (preamble & 0xFF)) ? DECODER_STATE_PREAMBLE_1 : DECODER_STATE_PREAMBLE_0;
                break;

            case DECODER_STATE_PREAMBLE_1:
                //std::cout << "DECODER_STATE_SYNC_1" << std::endl;
                state = (c == (preamble >> 8)) ? DECODER_STATE_CLASS_ID : DECODER_STATE_PREAMBLE_0;
                break;

            case DECODER_STATE_CLASS_ID:
                //std::cout << "DECODER_STATE_CLASS_ID" << std::endl;
                packet->messageId = c;
                state = DECODER_STATE_MSG_ID;
                break;

            case DECODER_STATE_MSG_ID:
                //std::cout << "DECODER_STATE_MSG_ID" << std::endl;
                packet->messageId += (c << 8);
                state = DECODER_STATE_LENGTH_0;
                break;

            case DECODER_STATE_LENGTH_0:
                //std::cout << "DECODER_STATE_LENGTH_0" << std::endl;
                packet->payloadLength = c;
                state = DECODER_STATE_LENGTH_1;
                break;

            case DECODER_STATE_LENGTH_1:
                //std::cout << "DECODER_STATE_LENGTH_1" << std::endl;
                packet->payloadLength += (c << 8);

                if(packet->payloadLength > payloadSize)
                {
                    return false;
                }

                payloadCounter = packet->payloadLength;
                if(packet->payloadLength > 0)
                {
                    state = DECODER_STATE_PAYLOAD;
                }
                else
                {
                    state = DECODER_STATE_CHECKSUM_0;
                }
                break;

            case DECODER_STATE_PAYLOAD:
                //std::cout << "DECODER_STATE_PAYLOAD" << std::endl;
                packet->payload[packet->payloadLength - payloadCounter--] = c;
                if(payloadCounter == 0)
                {
                    state = DECODER_STATE_CHECKSUM_0;
                }
                break;

            case DECODER_STATE_CHECKSUM_0:
                //std::cout << "DECODER_STATE_CHECKSUM_0" << std::endl;
                packet->checksum = c;
                state = DECODER_STATE_CHECKSUM_1;
                break;

            case DECODER_STATE_CHECKSUM_1:
                //std::cout << "DECODER_STATE_CHECKSUM_1" << std::endl;
                packet->checksum += (c << 8);
                state = DECODER_STATE_PREAMBLE_0;
                isWholePacket = true;
                std::cout << "Took " << std::dec << std::chrono::duration_cast<std::chrono::milliseconds>(d).count() << "ms" << std::endl;
                break;
            }
        }
        usleep(1000);
    }
    return isWholePacket;
}

bool Protocol::sendMessage(packet_t *packet) const
{
    uint8_t msg[payloadSize];

    memcpy(msg, packet, 6);
    memcpy(msg + 6, packet->payload, packet->payloadLength);
    memcpy(msg + 6 + packet->payloadLength, &packet->checksum, 2);

    std::cout << "RAW: ";
    for(uint16_t i = 0; i < 6 + packet->payloadLength + 2; ++i)
    {
        int n = msg[i];
        std::cout << "x" << std::hex << n << " ";
    }
    std::cout << std::endl;

    if(m_portfd)
    {
        write(m_portfd, msg, 6 + packet->payloadLength + 2);
    }
    return true;
}

bool Protocol::checkPacket(const packet_t *packet) const
{
    uint16_t cs = calculateCheckSum(packet);
    return (cs == packet->checksum);
}

uint16_t Protocol::calculateCheckSum(const packet_t *packet) const
{
    uint8_t a = 0;
    uint8_t b = 0;

    a = a + (packet->messageId & 0xFF);
    b = b + a;

    a = a + (packet->messageId >> 8);
    b = b + a;

    a = a + (packet->payloadLength & 0xFF);
    b = b + a;

    a = a + (packet->payloadLength >> 8);
    b = b + a;

    for(uint16_t i = 0; i < packet->payloadLength; ++i)
    {
        a = a + packet->payload[i];
        b = b + a;
    }

    return (b << 8) + a;
}

