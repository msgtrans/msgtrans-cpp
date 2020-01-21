/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#include <TcpTransportSession.h>
#include <future>
#include "TcpClientChannel.h"
#include "PacketSerizlizer.h"
#include "TransportSession.h"
#include "TransportContext.h"


TcpClientChannel::TcpClientChannel(std::string host, int port): m_host(host),m_port(port),m_isConnected(false){
    m_connector = std::make_shared<Tcp>();
}

void TcpClientChannel::connect()
{
    initialize();

    m_connector->connect(m_host,m_port);
}

void TcpClientChannel::initialize()
{
    m_connector->onConnect = [&](bool bSuccess)
    {
       // m_delegate->onConnect(bSuccess);
       m_isConnected = bSuccess;
       m_connector->setConnectionStatus(bSuccess);
    };

    m_connector->onMessage = [&](evpp::Buffer *msg)
    {
        size_t buffer_length = msg->length();
        char * buffer = new char [buffer_length];
        memset(buffer,0,buffer_length);
        memcpy(buffer,msg->data(),buffer_length);
       // const char* buffer = msg->data();
        msg->Skip(buffer_length);

        std::cout << "-------------received buffer-------------" << std::endl;
        std::cout << "buffer_length: " << buffer_length;
       // printBuffer((unsigned char *) buffer, buffer_length);

        std::vector<std::shared_ptr<MessageBuffer>> packages;
        bool ret = m_message_serizlizer.DeSerialize(buffer, buffer_length, packages);
        if(!ret)
        {
            std::cout << "failed to deSerialize net buffer";
        }

        for(int i =0; i < packages.size(); i++)
        {
            dispatchMessage(packages[i]);
        }
    };
}

void TcpClientChannel::dispatchMessage(std::shared_ptr<MessageBuffer>& message)
{
    std::shared_ptr<CRequest> pRequest(TheReqFactoryRef.CreateRequest(message->m_id));
    if (pRequest)
    {
        TcpTransportSession* session = (TcpTransportSession*)m_connector->getAttribute(CHANNELSESSION);
        if (!session)
        {
            session = new TcpTransportSession(0,m_connector);
            m_connector->setAttribute(CHANNELSESSION,session);
        }
        TransportContext context(session);
        pRequest->ProcRequest(context,message);
    }
}

bool TcpClientChannel::isConnected()
{
    return m_connector->isConnected();
}

void TcpClientChannel::send(MessageBuffer& buff)
{
    if (m_connector)
    {
        std::shared_ptr<char> packet_buff;
        uint32_t packet_length =  PacketSerizlizer::Serialize(buff , packet_buff);
        if (packet_buff && packet_length)
        {
            m_connector->sendMessage(packet_buff.get(), packet_length);
        }
    }
}

void TcpClientChannel::close()
{
    if (m_connector)
    {
        m_connector->close();
    }
}