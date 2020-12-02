//
// Created by linsen on 2019/12/10.
//

#ifndef MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H
#define MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H

#include "ClientChannel.h"
#include <string>
#include <memory>
#include "ee2e/crypto.h"

using namespace crypto;

class ClientChannel;

class MessageTransportClient : public std::enable_shared_from_this<MessageTransportClient> {

public:
    static bool ee2e ;
    ownkey_s  client_key;
    peerkey_s server_key;

private:
    bool m_isConnected = false;
    std::string m_name;
    std::shared_ptr<ClientChannel> m_channel;

public:
    MessageTransportClient(std::string name , bool ee2e = false);

    void transport(std::shared_ptr<ClientChannel>& m_channel);

    void send(std::shared_ptr<MessageBuffer> buffer);

    void close();

};



#endif //MSGTRANS_CLIENT_MESSAGETRANSPORTCLIENT_H
