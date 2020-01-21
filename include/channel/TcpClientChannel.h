/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_TCPCLIENTCHANNEL_H
#define MSGTRANS_CLIENT_TCPCLIENTCHANNEL_H

#include "ClientChannel.h"
#include "MessageTransport.h"
#include "PacketSerizlizer.h"
#include "Tcp.h"

#define CHANNELSESSION  "ChannelSession"

class TcpClientChannel : public  ClientChannel {

private:
    std::string m_host;
    int m_port;
    MessageTransportClient* m_messageTransport;
    std::shared_ptr<ConnectionBase> m_connector;
    PacketSerizlizer m_message_serizlizer;
    bool m_isConnected;

public:
    TcpClientChannel(std::string host, int port);
    void set(MessageTransportClient* transport){m_messageTransport = transport ;}
    void connect();
    bool isConnected();
    void send(MessageBuffer& buff);
    void close();
private:
    void initialize();
    void dispatchMessage(std::shared_ptr<MessageBuffer>& message);
};


#endif //MSGTRANS_CLIENT_TCPCLIENTCHANNEL_H
