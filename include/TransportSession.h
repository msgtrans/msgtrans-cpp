//
// Created by linsen on 2019/12/11.
//

#ifndef MSGTRANS_CLIENT_TRANSPORTSESSION_H
#define MSGTRANS_CLIENT_TRANSPORTSESSION_H

#include "MessageBuffer.h"
#include <string>

class TransportSession {

private:
    long m_id;

public:
    TransportSession(long id);
    virtual void setAttribute(std::string key, void *data) {}
    virtual void* getAttribute(std::string key) { return NULL;}

    virtual void send(MessageBuffer& buffer) {}

    void send(uint32_t messageId,std::string content)
    {
        MessageBuffer buffer(messageId , (char*)content.c_str());
        send(buffer);
    }

    virtual void close() {}

    virtual bool isConnected() {}
};


#endif //MSGTRANS_CLIENT_TRANSPORTSESSION_H
