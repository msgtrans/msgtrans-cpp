//
// Created by linsen on 2019/12/10.
//

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
    void send(std::shared_ptr<MessageBuffer> buff);
    void close();
    void setOnConnect(const std::function<void(bool)>& cb)
    {
        m_onConnect = cb;
    }
    void setCloseCallBack(const std::function<void(bool)>& cb)
    {
        m_onClosed = cb;
    }
private:
    void initialize();
    void dispatchMessage(std::shared_ptr<MessageBuffer>& message);
    void keyExchangeInitiate();
    void keyExchangeRequest(std::shared_ptr<MessageBuffer> message);

    std::function<void(bool)> m_onConnect;
    std::function<void(bool)> m_onClosed;
};


#endif //MSGTRANS_CLIENT_TCPCLIENTCHANNEL_H
