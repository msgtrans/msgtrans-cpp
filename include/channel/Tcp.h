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

using namespace evpp;

class  mTcpConn : public evpp::TCPConn , public std::enable_shared_from_this<mTcpConn>
{
public:
    mTcpConn(EventLoop* loop,
    const std::string& name,
    evpp_socket_t sockfd,
    const std::string& laddr,
    const std::string& raddr,
            uint64_t id) : TCPConn(loop,name,sockfd,laddr,raddr,id)
    {

    }
    void SetCloseCallback(CloseCallback cb)
    {
        SetCloseCallback(cb);
    }

};

typedef std::shared_ptr<mTcpConn> mTCPConnPtr;

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
    evpp::TCPClient* m_client;
    bool m_isConnected = false;
    evpp::EventLoop m_loop;
};

#endif //MSGTRANS_CLIENT_TCP_H
