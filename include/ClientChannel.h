/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_CLIENTCHANNEL_H
#define MSGTRANS_CLIENT_CLIENTCHANNEL_H

#include <memory>
#include "MessageBuffer.h"
#include "MessageTransportClient.h"

class MessageTransportClient;

class ClientChannel : public std::enable_shared_from_this<ClientChannel> {

public:
    virtual  void connect() = 0;
    virtual  bool isConnected() = 0;
    virtual  void set(MessageTransportClient* transport) = 0;
    virtual  void send(MessageBuffer& buff) = 0;
    virtual  void close() = 0;
};


#endif //MSGTRANS_CLIENT_CLIENTCHANNEL_H
