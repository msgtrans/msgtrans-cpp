//
// Created by linsen on 2019/12/10.
//

#include <future>
#include "Tcp.h"

#include <iostream>
#include <functional>
#include <future>
#include <thread>
#include <chrono>
#include <cstdlib>
void Tcp::sendMessage(std::string msg)
{
    m_connection->Send(msg);
}

void Tcp::sendMessage(const void* d, size_t dlen)
{
    m_mutex.lock();
    if (m_connection == nullptr || m_connection->IsDisconnected())
    {
        if(m_onClosed != nullptr)
        {
            m_onClosed();
        }
    }else
    {
        m_connection->Send(d, dlen);
    }
    m_mutex.unlock();
}

void Tcp::close()
{
    //m_connection->Close();
    //m_mutex.lock();
    if (!m_connection->IsDisconnected())
    {
        m_client->Disconnect();
    }
    //std::cout << "close!!" << std::endl;
    m_loop.Stop();
    //m_mutex.unlock();
}

void Tcp::threadSetPromise(std::string ip, int port, std::promise<bool>& promiseObj) {
    std::string addr = ip + ":" + std::to_string(port);
    //evpp::EventLoop loop;
    //m_mutex.unlock();
    m_client = new evpp::TCPClient(&m_loop, addr, "imsdk");
    m_client->set_auto_reconnect(false);
//    client.conn()->SetCloseCallback([&](const evpp::TCPConnPtr &conn){
//
//    });
    m_client->SetMessageCallback([this](const evpp::TCPConnPtr &conn,
                                                     evpp::Buffer *msg)
                              {
                                  assert(onMessage);
                                  onMessage(msg);
                              });

    m_client->SetConnectionCallback([&promiseObj,this](const evpp::TCPConnPtr &conn)
                                 {
                                     assert(conn);
                                     assert(onConnect);
                                     if (conn->IsConnected())
                                     {
                                         std::cout << "Connected to " << conn->remote_addr()
                                                   << std::endl;

                                         m_connection = conn;
                                         onConnect(true);
                                         promiseObj.set_value(true);
                                     }
                                     else
                                     {
                                         std::cout << "connect  error"  << std::endl;
                                         onConnect(false);
                                         m_connection = nullptr;
                                         //m_loop.Stop();
                                         //promiseObj.set_value(true);
                                     }
                                 });
    m_client->Connect();
    m_loop.Run();
    //m_mutex.unlock();
}

void Tcp::connect(std::string ip, int port)
{
    std::promise<bool> promiseObj;
    std::future<bool> futureObj = promiseObj.get_future();
    std::thread t(&Tcp::threadSetPromise, this, ip , port, std::ref(promiseObj));
    t.detach();
    futureObj.get();
}