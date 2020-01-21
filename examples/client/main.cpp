/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#include "MessageTransportClient.h"
#include "TcpClientChannel.h"
#include "MessageBuffer.h"
#include "CRequest.h"
#include "MessageTransport.h"

enum MESSAGE {
    HELLO = 10001,
    WELCOME = 10002
};

class HelloWorld : public CRequest
{
public:
    void ProcRequest(TransportContext& context ,std::shared_ptr<MessageBuffer>& messageBuffer)
    {
        MessageBuffer resp(MESSAGE::WELCOME, (char*)"welcome");
        context.session()->send(resp);
    }
};

RegisterRequest(MESSAGE::HELLO, HelloWorld)

int main()
{
    try {
        std::shared_ptr<MessageTransportClient> client = std::make_shared<MessageTransportClient>("test");

        std::shared_ptr<ClientChannel> channel = std::make_shared<TcpClientChannel>("10.1.223.99", 9001);
        client->transport(channel);


        std::string msg = "hello";
        MessageBuffer buffer(MESSAGE::HELLO, (char *) msg.c_str());

        client->send(buffer);

        getchar();
        client->close();
    }
    catch (std::exception& e)
    {
        // What to do?
    }
}
