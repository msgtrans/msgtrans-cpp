/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_TCPTRANSPORTSESSION_H
#define MSGTRANS_CLIENT_TCPTRANSPORTSESSION_H

#include "TransportSession.h"
#include "ConnectionBase.h"

class TcpTransportSession : public TransportSession {

private:
    std::shared_ptr<ConnectionBase> m_conn;
public:
    TcpTransportSession(u_long id,std::shared_ptr<ConnectionBase>& conn);

    virtual  void setAttribute(std::string key, void *data);
    virtual  void* getAttribute(std::string key);
    virtual  void send(MessageBuffer& buffer);
    virtual  void close();
    virtual  bool isConnected();
};


#endif //MSGTRANS_CLIENT_TCPTRANSPORTSESSION_H
