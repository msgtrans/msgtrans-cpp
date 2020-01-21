/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H
#define MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H

#include "ClientChannel.h"
#include <string>
#include <memory>

class ClientChannel;

class MessageTransportClient : public std::enable_shared_from_this<MessageTransportClient> {

private:
    bool m_isConnected = false;
    std::string m_name;
    std::shared_ptr<ClientChannel> m_channel;

public:
    MessageTransportClient(std::string name);

    void transport(std::shared_ptr<ClientChannel>& m_channel);

    void send(MessageBuffer& buffer);

    void close();

};


#endif //MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H
