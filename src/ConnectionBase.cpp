/*
 * Copyright (c) 2020 HuntLabs
 *
 * Homepage: https://www.huntlabs.net
 * 
 */

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