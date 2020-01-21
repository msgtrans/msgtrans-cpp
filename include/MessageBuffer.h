/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_MESSAGEBUFFER_H
#define MSGTRANS_CLIENT_MESSAGEBUFFER_H


#include <stdint.h>

struct MessageBuffer {
    uint32_t m_id;
    char* m_data;

    MessageBuffer(uint32_t id,   char* data)
    {
        m_id = id;
        m_data = data;
    }

    ~MessageBuffer()
    {
        if(m_data)
        {
            delete[] m_data;
        }
    }
};


#endif //MSGTRANS_CLIENT_MESSAGEBUFFER_H
