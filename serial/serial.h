#ifndef __SERIAL_INTERFACE_H_
#define __SERIAL_INTERFACE_H_

#include <boost/asio.hpp>


class SerialInterface
{
public:
    SerialInterface( std::string port, unsigned int baud );
    ~SerialInterface();

    void write( const uint8_t *buffer, size_t length );
    size_t read( uint8_t *c );

private:
    boost::asio::serial_port m_serial;
    boost::asio::io_service m_io;
};

#endif
