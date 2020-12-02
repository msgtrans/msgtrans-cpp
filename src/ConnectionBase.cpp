//
// Created by linsen on 2019/12/11.
//

#include "ConnectionBase.h"

void ConnectionBase::setAttribute(std::string key, void *data)
{
    if(data)
    {
        m_map[key] = data;
    }
}

void* ConnectionBase::getAttribute(std::string key)
{
    auto iter = m_map.find(key);
    if (iter != m_map.end())
    {
        return  iter->second;
    }
    return  NULL;
}