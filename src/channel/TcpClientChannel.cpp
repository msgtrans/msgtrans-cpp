/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#include <TcpTransportSession.h>
#include <future>
#include "TcpClientChannel.h"
#include "PacketSerizlizer.h"
#include "TransportSession.h"
#include "TransportContext.h"
#include "ee2e/key_exchange.pb.h"
#include "ee2e/common.h"
#include "common/base64.h"

bool MessageTransportClient::ee2e = false;

TcpClientChannel::TcpClientChannel(std::string host, int port): m_host(host),m_port(port),m_isConnected(false){
    m_connector = std::make_shared<Tcp>();
}

void TcpClientChannel::connect()
{
    initialize();

    m_connector->connect(m_host,m_port);
}

void TcpClientChannel::initialize()
{
    m_connector->onConnect = [&](bool bSuccess)
    {
       // m_delegate->onConnect(bSuccess);
       m_isConnected = bSuccess;
       m_connector->setConnectionStatus(bSuccess);
        if(bSuccess && MessageTransportClient::ee2e)
        {
            keyExchangeInitiate();
        }
        if (!MessageTransportClient::ee2e && m_onConnect != nullptr)
        {
            m_onConnect(bSuccess);
        }
    };

    m_connector->onMessage = [&](evpp::Buffer *msg)
    {
        size_t buffer_length = msg->length();
        char * buffer = new char [buffer_length];
        memset(buffer,0,buffer_length);
        memcpy(buffer,msg->data(),buffer_length);
       // const char* buffer = msg->data();
        msg->Skip(buffer_length);

        std::cout << "-------------received buffer-------------" << std::endl;
        std::cout << "buffer_length: " << buffer_length;
       // printBuffer((unsigned char *) buffer, buffer_length);

        std::vector<std::shared_ptr<MessageBuffer>> packages;
        bool ret = m_message_serizlizer.DeSerialize(buffer, buffer_length, packages);
        if(!ret)
        {
            std::cout << "failed to deSerialize net buffer";
        }

        for(int i =0; i < packages.size(); i++)
        {
            if (MessageTransportClient::ee2e && (packages[i]->m_id == ee2e::MESSAGE::INITIATE || packages[i]->m_id == ee2e::MESSAGE::FINALIZE))
            {
                keyExchangeRequest(packages[i]);
            } else
            {
            dispatchMessage(packages[i]);
        }
        }
    };
}


void TcpClientChannel::keyExchangeRequest(std::shared_ptr<MessageBuffer> message)
{
    switch(message->m_id)
    {
        case ee2e::MESSAGE::INITIATE :
        {
            ee2e::KeyExchangeRequest response;

            response.ParsePartialFromString(message->m_data);

            std::string ec_pub_key = response.key_info().ec_public_key_65bytes();
            std::string salt = response.key_info().salt_32bytes();

            std::string d_ec_pub_key;
            std::string d_salt;
            Base64::Decode(ec_pub_key,&d_ec_pub_key);
            Base64::Decode(salt,&d_salt);

            memcpy(m_messageTransport->server_key.ec_pub_key,d_ec_pub_key.c_str(), sizeof(m_messageTransport->server_key.ec_pub_key));
            memcpy(m_messageTransport->server_key.salt,d_salt.c_str(), sizeof(m_messageTransport->server_key.salt));
           // memcpy(m_messageTransport->server_key.ec_pub_key, response.key_info().ec_public_key_65bytes().data(), response.key_info().ec_public_key_65bytes().size());
            //memcpy(m_messageTransport->server_key.salt, response.key_info().salt_32bytes().data(), response.key_info().salt_32bytes().size());

//            printf("server before ec_pub : ");
//            for(int i = 0 ; i < response.key_info().ec_public_key_65bytes().size() ; ++i)
//            {
//                printf("%d " ,response.key_info().ec_public_key_65bytes().data()[i]);
//            }
//            printf("\n");
//
//
//            printf("server ec_pub_key : ");
//            for(int i = 0 ; i < sizeof(m_messageTransport->server_key.ec_pub_key) ; ++i)
//            {
//                 printf("%d " ,m_messageTransport->server_key.ec_pub_key[i]);
//            }
//            printf("\n");
//
//
//            printf("server salt : ");
//            for(int i = 0 ; i < sizeof(m_messageTransport->server_key.salt) ; ++i)
//            {
//                printf("%d " ,m_messageTransport->server_key.salt[i]);
//            }
//            printf("\n");
//            m_messageTransport->server_key.ec_pub_key = keyExchangeRes.key_info.ec_public_key_65bytes;
//            MessageTransportClient.server_key.salt = keyExchangeRes.key_info.salt_32bytes;

            //logInfo("server pub : %s" , MessageTransportClient.server_key.ec_pub_key);
            //logInfo("service salt : %s", MessageTransportClient.server_key.salt);

            if (common::key_calculate(m_messageTransport->client_key,m_messageTransport->server_key))
            {
                std::shared_ptr<MessageBuffer> buffer = std::make_shared<MessageBuffer>(ee2e::MESSAGE::FINALIZE,"");
                send(buffer);
            }else
            {
                std::cerr << "keyCalculate error" << std::endl;
            }

            printf("client Key : ");
            for(int i = 0 ; i < sizeof(m_messageTransport->server_key.aes_key) ; ++i)
            {
                printf("%d " ,m_messageTransport->server_key.aes_key[i]);
            }
            printf("\n");
            break;


        }
        case ee2e::MESSAGE::FINALIZE :
        {
            std::cout << "======================Key exchange completed======================" << std::endl;
            if(m_onConnect != nullptr)
            {
                m_onConnect(true);
            }
            break;
        }
        default : break;
    }

}



void TcpClientChannel::keyExchangeInitiate()
{
    ee2e::KeyExchangeRequest key_exchange_request;
    ee2e::KeyInfo*  key_info  = new ee2e::KeyInfo();

    /* send ownkey to server */
    std::string pub(m_messageTransport->client_key.ec_pub_key , m_messageTransport->client_key.ec_pub_key + sizeof(m_messageTransport->client_key.ec_pub_key));
    std::string out_pub;
    std::string salt(m_messageTransport->client_key.salt,m_messageTransport->client_key.salt + sizeof(m_messageTransport->client_key.salt));
    std::string out_salt;
    Base64::Encode(pub,&out_pub);
    Base64::Encode(salt,&out_salt);
//    Base64::Encode((char*)m_messageTransport->client_key.salt, sizeof(sizeof(crypto::ownkey_s::salt)), (char*)salt.c_str(),);
    key_info->set_ec_public_key_65bytes(out_pub);
    //key_info->set_ec_public_key_65bytes(m_messageTransport->client_key.ec_pub_key, sizeof(crypto::ownkey_s::ec_pub_key));
    //key_info->set_salt_32bytes(m_messageTransport->client_key.salt, sizeof(crypto::ownkey_s::salt));
    key_info->set_salt_32bytes(out_salt);
    std::cout << ">>>>Send client's own key to server:" << std::endl;
    std::cout << "  ECDH-PUB-KEY:" << std::endl;

//    printf("client ec_pub_key : ");
//    for(int i = 0 ; i < sizeof(m_messageTransport->client_key.ec_pub_key) ; ++i)
//    {
//        printf(" %d  " ,m_messageTransport->client_key.ec_pub_key[i]);
//    }
//    printf("\n");
//
//
//    printf("client salt : ");
//    for(int i = 0 ; i < sizeof(m_messageTransport->client_key.salt) ; ++i)
//    {
//        printf(" %d  " ,m_messageTransport->client_key.salt[i]);
//    }
//    printf("\n");

    std::string str_request;
    key_exchange_request.set_allocated_key_info(key_info);
    key_exchange_request.set_key_exchange_type(ee2e::KeyExchangeType::KEY_EXCHANGE_INITIATE);
    key_exchange_request.SerializeToString(&str_request);

    std::shared_ptr<MessageBuffer> buffer = std::make_shared<MessageBuffer>(ee2e::MESSAGE::INITIATE,str_request);
    send(buffer);
}

void TcpClientChannel::dispatchMessage(std::shared_ptr<MessageBuffer>& message)
{
    std::shared_ptr<CRequest> pRequest(TheReqFactoryRef.CreateRequest(message->m_id));
    if (pRequest)
    {
        TcpTransportSession* session = (TcpTransportSession*)m_connector->getAttribute(CHANNELSESSION);
        if (!session)
        {
            session = new TcpTransportSession(0,m_connector);
            m_connector->setAttribute(CHANNELSESSION,session);
        }
        TransportContext context(session);
        if(MessageTransportClient::ee2e)
        {
            if(!common::encrypted_decode(message,m_messageTransport->server_key))
            {
                return;
            }
        }
        pRequest->ProcRequest(context,message);
    }
}

bool TcpClientChannel::isConnected()
{
    return m_connector->isConnected();
}

void TcpClientChannel::send(std::shared_ptr<MessageBuffer> buff)
{
    if (m_connector)
    {
        //std::shared_ptr<char> packet_buff;
        if (MessageTransportClient::ee2e && (buff->m_id != ee2e::MESSAGE::INITIATE  && buff->m_id != ee2e::MESSAGE::FINALIZE))
        {
            if(!common::encrypted_encode(buff ,m_messageTransport->client_key,m_messageTransport->server_key))
            {
                return;
            }
        }

        char* packet_buff;
        uint32_t packet_length =  PacketSerizlizer::Serialize(buff , packet_buff);
        if (packet_buff && packet_length)
        {
            m_connector->sendMessage(packet_buff, packet_length);
            delete []  packet_buff;
        }
    }
}

void TcpClientChannel::close()
{
    if (m_connector)
    {
        m_connector->close();
    }
}