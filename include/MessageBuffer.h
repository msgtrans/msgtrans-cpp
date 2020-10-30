/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

#ifndef MSGTRANS_CLIENT_MESSAGEBUFFER_H
#define MSGTRANS_CLIENT_MESSAGEBUFFER_H


#include <stdint.h>
#include <cstring>
#include <memory>
#include <iostream>
struct MessageBuffer {
    uint32_t m_id;
    std::string m_data;
    MessageBuffer(uint32_t id, const std::string& data)
    {
        m_id = id;
//        size_t len = strlen(data);
//        std::cout  << "m_data length :" << len  << std::endl;
        m_data = data;
        //memcpy(m_data, data, len);
    }

    ~MessageBuffer()
    {
//        if(m_data)
//        {
//            delete[] m_data;
//        }
//        m_data = nullptr;
    }
};


#endif //MSGTRANS_CLIENT_MESSAGEBUFFER_H
