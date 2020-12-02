//
// Created by linsen on 2019/12/10.
//

#include <string>
#include <memory>
#include "MessageTransportClient.h"

MessageTransportClient::MessageTransportClient(std::string name , bool ee2e) : m_name(name)
{
    MessageTransportClient::ee2e = ee2e;
    if (ee2e)
    {
        if (!crypto::generate_ecdh_keys(client_key.ec_pub_key, client_key.ec_priv_key))
        {
            std::cerr << "ECDH-KEY generation failed." << std::endl;
        }
        if (!crypto::rand_salt(client_key.salt, sizeof(crypto::ownkey_s::salt)))
        {
            std::cerr << "Random salt generation failed." << std::endl;
        }
    }
}

void MessageTransportClient::transport(std::shared_ptr<ClientChannel>& channel)
{
    m_channel = channel;
    m_channel->set(this);
    m_channel->connect();
}

void MessageTransportClient::send(std::shared_ptr<MessageBuffer> buffer)
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