/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

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
    m_connection->Send(d, dlen);
}

void Tcp::close()
{
    m_connection->Close();
}

void Tcp::threadSetPromise(std::string ip, int port, std::promise<bool>& promiseObj) {
    std::string addr = ip + ":" + std::to_string(port);
    evpp::EventLoop loop;
    evpp::TCPClient client(&loop, addr, "imsdk");
    client.SetMessageCallback([&loop, &client, this](const evpp::TCPConnPtr &conn,
                                                     evpp::Buffer *msg)
                              {
                                  assert(onMessage);
                                  onMessage(msg);
                              });

    client.SetConnectionCallback([&](const evpp::TCPConnPtr &conn)
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
                                         onConnect(false);
                                     }
                                 });
    client.Connect();
    loop.Run();
}

void Tcp::connect(std::string ip, int port)
{
    std::promise<bool> promiseObj;
    std::future<bool> futureObj = promiseObj.get_future();
    std::thread t(&Tcp::threadSetPromise, this, ip , port, std::ref(promiseObj));
    t.detach();
    futureObj.get();
}