/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_TRANSPORTCONTEXT_H
#define MSGTRANS_CLIENT_TRANSPORTCONTEXT_H

#include "TransportSession.h"

class TransportContext {

private:
    TransportSession* m_currentSession;
public:
    TransportContext(TransportSession* session)
    {
        m_currentSession = session;
    }

    TransportSession* session()
    {
        return m_currentSession;
    }
};


#endif //MSGTRANS_CLIENT_TRANSPORTCONTEXT_H
