//
// Created by linsen on 2019/12/11.
//

#include "channel/TcpTransportSession.h"
#include "PacketSerizlizer.h"

TcpTransportSession::TcpTransportSession(u_long id,std::shared_ptr<ConnectionBase>& conn) : TransportSession(id) , m_conn(conn)
{

}

void TcpTransportSession::send(std::shared_ptr<MessageBuffer> buffer)
{
    if (m_conn && m_conn->isConnected())
    {
        char* packet_buff;
        uint32_t packet_length =  PacketSerizlizer::Serialize(buffer , packet_buff);
        if (packet_buff && packet_length)
        {
            m_conn->sendMessage(packet_buff, packet_length);
            delete [] packet_buff;
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