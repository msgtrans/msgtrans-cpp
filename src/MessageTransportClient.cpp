/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#include <string>
#include <memory>
#include "MessageTransportClient.h"

MessageTransportClient::MessageTransportClient(std::string name) : m_name(name)
{

}

void MessageTransportClient::transport(std::shared_ptr<ClientChannel>& channel)
{
    m_channel = channel;
    m_channel->set(this);
    m_channel->connect();
}

void MessageTransportClient::send(MessageBuffer& buffer)
{
    m_channel->send(buffer);
}

void MessageTransportClient::close()
{
    if (m_channel)
    {
        m_channel->close();
    }
}