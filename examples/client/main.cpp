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
#include "chat.message.pb.h"

enum MESSAGE {
    HELLO = 100,
    WELCOME = 200
};

class HelloWorld : public CRequest
{
public:
    void ProcRequest(TransportContext& context ,std::shared_ptr<MessageBuffer>& messageBuffer)
    {
//        MessageBuffer resp(MESSAGE::WELCOME, (char*)"welcome");
//        context.session()->send(resp);
          std::cout << "msg:" << messageBuffer->m_data << std::endl;
    }
};
RegisterRequest(MESSAGE::WELCOME, HelloWorld)

int main()
{
    try {
        std::shared_ptr<MessageTransportClient> client = std::make_shared<MessageTransportClient>("test" ,true);

        std::shared_ptr<ClientChannel> channel = std::make_shared<TcpClientChannel>("10.1.223.99", 9001);
        client->transport(channel);

        ChatPb::Message pbMessage;
        pbMessage.set_chatid("1234");
        pbMessage.set_message("gggggssfsdfsdfsdfdsfsadfafdafewr");
        pbMessage.set_fromuserid("80000");
        pbMessage.set_type((ChatPb::MsgType)0);
        pbMessage.set_msgtime(0);
        pbMessage.set_localid("");
        pbMessage.set_messageid("345345");
        std::string msgBuff;
        pbMessage.SerializeToString(&msgBuff);

//        std::string msg = "fdgdgfkkkkkkkkkkkkkkkkkkkkkkk0kdk34k00000000kkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkkk435345#$%#$^&#&TYHTYRYTtyrytrtryeTYTYRYTWREEWEWREWFDFSDFDWFDFASDFSAFASEFASFDAWFSDGAGFSDGSERSTG#W^T$TGGTRGRSTH#%^T^ERTERFGEWRAEAEWRFWAEFFEWAFFEW";
        std::shared_ptr<MessageBuffer> buffer = std::make_shared<MessageBuffer>(MESSAGE::HELLO, msgBuff);

        sleep(1);
        client->send(buffer);

        getchar();
        client->close();
    } catch (std::exception& e)
    {

    }
}