#include "serial.h"
#include <boost/asio.hpp>
#include <iostream>

SerialInterface::SerialInterface( std::string port, unsigned int baud )
    : m_io(), m_serial(m_io, port)
{
    m_serial.set_option(boost::asio::serial_port_base::baud_rate(baud));
}


SerialInterface::~SerialInterface()
{

}


void SerialInterface::write( const uint8_t *buffer, size_t length )
{
    boost::asio::write(m_serial, boost::asio::buffer((const char*)buffer, length));
}

size_t SerialInterface::read( uint8_t *c )
{
    return boost::asio::read(m_serial, boost::asio::buffer((unsigned char *)c, 1));
}