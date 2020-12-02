//
// Created by linsen on 2019/12/10.
//

#ifndef MSGTRANS_CLIENT_MESSAGEBUFFER_H
#define MSGTRANS_CLIENT_MESSAGEBUFFER_H


#include <stdint.h>
#include <cstring>
#include <memory>
#include <iostream>

struct Extend
{
    uint32_t requestId;
    uint32_t responseId;
};


class MessageBuffer {

public:
    MessageBuffer(const uint32_t& id, const std::string& data)
    {
        m_compression = 0;
        m_id = id;
        m_data = data;
        hasExtend = false;
    }

    MessageBuffer(const uint32_t& id, const std::string& data, const Extend& extend)
    {
        m_compression = 0;
        m_id = id;
//        m_tagId = tagId;
//        m_extendLength = sizeof(uint32_t);
        m_data = data;
        m_extend = extend;
        hasExtend = true;
    }

    ~MessageBuffer()
    {
//        if(m_data)
//        {
//            delete[] m_data;
//        }
//        m_data = nullptr;
    }
    unsigned char m_compression;
    uint32_t m_id;
    //uint32_t m_tagId;
    //uint32_t m_extendLength;
    std::string m_data;
    Extend m_extend;
    bool hasExtend;
};


#endif //MSGTRANS_CLIENT_MESSAGEBUFFER_H
