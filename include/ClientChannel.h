/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_CLIENTCHANNEL_H
#define MSGTRANS_CLIENT_CLIENTCHANNEL_H

#include <memory>
#include <functional>
#include "MessageBuffer.h"
#include "MessageTransportClient.h"

class MessageTransportClient;

class ClientChannel : public std::enable_shared_from_this<ClientChannel> {

public:
    virtual  void connect() = 0;
    virtual  bool isConnected() = 0;
    virtual  void set(MessageTransportClient* transport) = 0;
    virtual  void send(std::shared_ptr<MessageBuffer> buff) = 0;
    virtual  void close() = 0;
    virtual void setOnConnect(const std::function<void(bool)>& cb) = 0;
    virtual void setCloseCallBack(const std::function<void()>& cb) = 0;
};


#endif //MSGTRANS_CLIENT_CLIENTCHANNEL_H
