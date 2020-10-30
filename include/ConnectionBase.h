/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_CONNECTIONBASE_H
#define MSGTRANS_CLIENT_CONNECTIONBASE_H

#include <evpp/buffer.h>
#include <map>

class ConnectionBase {

private:
    std::map<std::string, void *> m_map;

public:
    ConnectionBase()
    {
    }

    virtual  void connect(std::string ip, int port) = 0;
    virtual  void sendMessage(std::string msg) = 0;
    virtual  void sendMessage(const void* d, size_t dlen) = 0;
    virtual  bool isConnected() = 0;
    virtual  void setConnectionStatus(bool status) = 0;
    virtual  void close() = 0;
    void setCloseCallBack(const std::function<void()>& cb)
    {
        m_onClosed = cb;
    }
    std::function<void(bool)> onConnect;
    std::function<void(evpp::Buffer *)> onMessage;

    void setAttribute(std::string key, void *data);
    void* getAttribute(std::string key);

    std::function<void()> m_onClosed;
};


#endif //MSGTRANS_CLIENT_CONNECTIONBASE_H
