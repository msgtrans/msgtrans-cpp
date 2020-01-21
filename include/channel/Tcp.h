/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_TCP_H
#define MSGTRANS_CLIENT_TCP_H

#include <evpp/tcp_client.h>
#include <evpp/buffer.h>
#include <evpp/tcp_conn.h>
#include <future>
#include "ConnectionBase.h"

class Tcp : public ConnectionBase
{
public:
    Tcp()
    {
    }

    void connect(std::string ip, int port);
    void sendMessage(std::string msg);
    void sendMessage(const void* d, size_t dlen);
    void close();
//    std::function<void(bool)> onConnect;
//    std::function<void(evpp::Buffer *)> onMessage;
    bool isConnected() { return m_isConnected ;}
    void setConnectionStatus(bool status) {m_isConnected = status ;}

private:
    void threadSetPromise(std::string ip, int port, std::promise<bool>& promiseObj);

private:
    evpp::TCPConnPtr m_connection;
    bool m_isConnected = false;
};

#endif //MSGTRANS_CLIENT_TCP_H
