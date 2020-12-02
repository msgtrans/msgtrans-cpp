//
// Created by linsen on 2019/12/10.
//

#include "MessageTransport.h"


CRequest* MessageTransport::CreateRequest(u_int32_t key)
{
    std::unordered_map<u_int32_t,CreateReqFunc>::iterator it = m_mapFunc.find(key);
    if (it != m_mapFunc.end())
    {
        return (it->second)();
    }
    else
    {
        std::cout<<__FUNCTION__<<" "<<__LINE__<<" failed to create request type: "<<key<<std::endl;
    }
    return NULL;
}

void MessageTransport::RegisterReq( u_int32_t key,CreateReqFunc func )
{
    if (m_mapFunc.find(key) != m_mapFunc.end())
    {
        std::cout<<__FUNCTION__<<" reqeust type: "<<key<<" has been registered "<<std::endl;
    }

    std::cout<<__FUNCTION__<<" "<<__LINE__<<" register request "<<key<<std::endl;

    m_mapFunc[key] = func;
}