/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#include "channel/TcpTransportSession.h"
#include "PacketSerizlizer.h"

TcpTransportSession::TcpTransportSession(u_long id,std::shared_ptr<ConnectionBase>& conn) : TransportSession(id) , m_conn(conn)
{

}

void TcpTransportSession::send(MessageBuffer& buffer)
{
    if (m_conn && m_conn->isConnected())
    {
        std::shared_ptr<char> packet_buff;
        uint32_t packet_length =  PacketSerizlizer::Serialize(buffer , packet_buff);
        if (packet_buff && packet_length)
        {
            m_conn->sendMessage(packet_buff.get(), packet_length);
        }
    }
}

void TcpTransportSession::close()
{
    if (m_conn && m_conn->isConnected())
    {
        m_conn->close();
    }
}

bool TcpTransportSession::isConnected()
{
    if (m_conn)
    {
        return m_conn->isConnected();
    }
    return  false;
}

void TcpTransportSession::setAttribute(std::string key, void *data)
{
    if (m_conn)
    {
        m_conn->setAttribute(key,data);
    }
}

void* TcpTransportSession::getAttribute(std::string key)
{
    if (m_conn)
    {
        return m_conn->getAttribute(key);
    }
    return NULL;
}