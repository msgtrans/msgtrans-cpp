//
// Created by linsen on 2019/12/10.
//

#ifndef MSGTRANS_CLIENT_CREQUEST_H
#define MSGTRANS_CLIENT_CREQUEST_H

#include <memory>
#include <iostream>
#include "TransportContext.h"
#include "MessageBuffer.h"

class CRequest : public std::enable_shared_from_this<CRequest>
{
public:

    virtual void ProcRequest(TransportContext& context ,std::shared_ptr<MessageBuffer>& messageBuffer)
    {

    }
    virtual ~CRequest()
    {

    }
    //pid_t	m_reqId;
};


#endif //MSGTRANS_CLIENT_CREQUEST_H
