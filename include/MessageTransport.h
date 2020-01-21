/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_MESSAGETRANSPORT_H
#define MSGTRANS_CLIENT_MESSAGETRANSPORT_H

#include <unordered_map>
#include "CRequest.h"
#include "Singleton.h"
#include <string>

class CRequest;

typedef CRequest* (*CreateReqFunc)();

class MessageTransport {

private:
    std::unordered_map<u_int32_t,CreateReqFunc>   m_mapFunc;

public:
    CRequest* CreateRequest(u_int32_t key);
    void      RegisterReq(u_int32_t key,CreateReqFunc func);

};

#define TheReqFactoryRef base::Singleton<MessageTransport>::instance()


class ReqRegister
{
public:
    ReqRegister(u_int32_t key,CreateReqFunc func)
    {
        TheReqFactoryRef.RegisterReq(key,func);
    }
};

#define RegisterRequest(key,requestName) \
class requestName##Register \
{\
public: \
	static CRequest* CreateInstance() \
	{ \
		requestName* pReq = new requestName();\
		return pReq; \
	};\
private: \
	static std::shared_ptr<ReqRegister> s_register; \
}; \
std::shared_ptr<ReqRegister> requestName##Register::s_register(new ReqRegister(key,requestName##Register::CreateInstance));

#endif //MSGTRANS_CLIENT_MESSAGETRANSPORT_H
