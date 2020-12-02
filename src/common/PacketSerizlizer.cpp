//
// Created by linsen on 2019/12/10.
//

#include "common/PacketSerizlizer.h"
#include <cstring>
#include <evpp/buffer.h>

PacketSerizlizer::PacketSerizlizer()
{
    m_last_save_data = new char[MAX_NETPACK_SIZE];
    memset(m_last_save_data, 0, MAX_NETPACK_SIZE);
    m_remain_size = 0;
}

uint32_t PacketSerizlizer::Serialize(std::shared_ptr<MessageBuffer> message, char*& serizelizeBuffer)
{
    //if (message->m_data.empty()) return 0;

    uint32_t slen = message->m_data.length();
    uint32_t extlen = 0;
    if(message->hasExtend)
    {
        extlen = sizeof(Extend);
    }
    //uint32_t extlen = message->m_extendLength;
    uint32_t length = HEADER_LENGTH + extlen + slen;

    serizelizeBuffer = new char[length];

    int offset = 0;
    memset(serizelizeBuffer, 0, length);

    // 小端转大端

    uint32_t message_id_bigendian = __builtin_bswap32(message->m_id);
    memcpy(serizelizeBuffer + offset, &message_id_bigendian,  sizeof(message_id_bigendian));
    offset += sizeof(message_id_bigendian);

    int32_t message_data_length_bigendian = __builtin_bswap32(slen);
    memcpy(serizelizeBuffer + offset, &message_data_length_bigendian, sizeof(message_data_length_bigendian));
    offset += sizeof(message_data_length_bigendian);


    unsigned char compression_data_bigendian = message->m_compression;
    memcpy(serizelizeBuffer + offset, &compression_data_bigendian, sizeof(compression_data_bigendian));
    offset += sizeof(compression_data_bigendian);


    int32_t extend_data_length = __builtin_bswap32(extlen);
    memcpy(serizelizeBuffer + offset, &extend_data_length, sizeof(extend_data_length));
    offset += sizeof(extend_data_length);

    if (message->hasExtend)
    {
        //int32_t bsTagId = __builtin_bswap32(message->m_tagId);
        memcpy(serizelizeBuffer + HEADER_LENGTH, &message->m_extend, extlen);
        memcpy(serizelizeBuffer + HEADER_LENGTH + extlen, message->m_data.c_str(), slen);
    } else{
        memcpy(serizelizeBuffer + HEADER_LENGTH, message->m_data.c_str(), slen);
    }

   // serizelizeBuffer = buffer;

   // delete [] message.m_data;

    return length;
}

bool PacketSerizlizer::DeSerialize(const char* data, int data_size,std::vector<std::shared_ptr<MessageBuffer>> &messages)
{
//        std::cout << "data_size: " << data_size;
//        printBuffer((char *) data, data_size);



    /* 本次接收到的数据拼接到上次数据 */
    memcpy( m_last_save_data + m_remain_size, data, data_size );
    m_remain_size += data_size;
    delete [] data;

    while(true)
    {
        int offset = 0;
        if ( m_remain_size >= HEADER_LENGTH)
        {
            //取出包头
//            char auth_key_id[8];
//            memset(auth_key_id, 0, 8);
//            memcpy(auth_key_id, m_last_save_data + offset, 8);
//            int64_t n_auth_key_id = __builtin_bswap64(bytesToInt64(auth_key_id));
//            offset += 8;

            char message_id[4];
            memset(message_id, 4, 0);
            memcpy(message_id, m_last_save_data + offset, 4);
            uint32_t n_message_id = __builtin_bswap32(bytesToInt32(message_id));
            offset += 4;

            char message_data_length[4];
            memset(message_data_length, 0, 4);
            memcpy(message_data_length, m_last_save_data + offset, 4);
            uint32_t n_message_data_length = __builtin_bswap32(bytesToInt32(message_data_length));
            offset += 4;

            unsigned char message_data_compression;
            memcpy(&message_data_compression, m_last_save_data + offset, 1);
            offset += 1;

            char message_data_extend_length[4];
            memset(message_data_extend_length, 0, 4);
            memcpy(message_data_extend_length, m_last_save_data + offset, 4);
            uint32_t n_message_data_extend_length= __builtin_bswap32(bytesToInt32(message_data_extend_length));
            offset += 4;


//                printBuffer(auth_key_id, 8);
//                printBuffer(message_id, 8);
//                printBuffer(message_data_length, 4);

//                std::cout << "n_auth_key_id: " << n_auth_key_id << std::endl;
           // std::cout << "n_message_id: " << n_message_id << std::endl;
//                std::cout << "n_message_data_length: " << n_message_data_length << std::endl;
            offset = offset < HEADER_LENGTH ? HEADER_LENGTH : offset; //填充预留字段

            /* 够一个数据包的长度 */
            if (m_remain_size >= n_message_data_length + n_message_data_extend_length + HEADER_LENGTH )
            {
                bool haveExtend = false;
                //uint32_t n_message_data_extend = 0;
                Extend extend;
                if(n_message_data_extend_length > 0)
                {
                    char message_data_extend[sizeof(Extend)];
                    memset(message_data_extend, 0, sizeof(Extend));
                    memcpy(message_data_extend, m_last_save_data + offset, sizeof(Extend));
                    memcpy(&extend,message_data_extend,sizeof(Extend));
                    //extend = (Extend*)message_data_extend;
                    //n_message_data_extend = __builtin_bswap32(bytesToInt32(message_data_extend));
                    offset += sizeof(Extend);
                    //std::cout << "DeSerialize tagId: " << n_message_data_extend << " n_message_data_extend_length: "<< n_message_data_extend_length << std::endl;
                    haveExtend = true;
                }

                char* message_data = new char[n_message_data_length+1];
                //std::shared_ptr<char> message_data = std::shared_ptr<char>(new char[n_message_data_length]);
               // char * message_data = new char[n_message_data_length];
                memset(message_data, 0, n_message_data_length + 1);
                memcpy(message_data, m_last_save_data + offset, n_message_data_length);
                offset += n_message_data_length;

                std::shared_ptr<MessageBuffer> msg;
                if (haveExtend)
                {
                    msg = std::make_shared<MessageBuffer> (n_message_id,std::string(message_data),extend);
                }else
                {
                    msg = std::make_shared<MessageBuffer> (n_message_id,std::string(message_data));
                }
               // msg.auth_key_id = n_auth_key_id;
               // msg.message_id = n_message_id;
                //msg.message_data_length = n_message_data_length;
               // msg.m_data = message_data;
                messages.push_back(msg);
                delete[] message_data;
                //取完一个完整的数据包后，整包数据从m_last_save_data移除
                m_remain_size = m_remain_size - n_message_data_extend_length - n_message_data_length - HEADER_LENGTH;
                if(m_remain_size > 0)
                {
                    char* left  =  new char[m_remain_size];
                    memset(left, 0, m_remain_size);
                    memcpy(left, m_last_save_data + offset, m_remain_size);

                    memset(m_last_save_data, 0, MAX_NETPACK_SIZE);
                    memcpy(m_last_save_data, left, m_remain_size);
                    delete [] left;
                }
                else
                {
                    //正好是一个完整包
                    memset(m_last_save_data, 0, MAX_NETPACK_SIZE);
//                        break;
                }
            }
            else
            {
                /* 不够一个数据包的长度 等下一个包 */
                break;

            }
        }
        else
        {
            /* 不够一个数据包头的长度 等下一个包 */
            break;
        }

    }
    return true;
}

int32_t PacketSerizlizer::bytesToInt32(char* bytes)
{
    return ((0xff & (int64_t)*(bytes+0)) << 0)
           | ((0xff & (int64_t)*(bytes+1)) << 8)
           | ((0xff & (int64_t)*(bytes+2)) << 16)
           | ((0xff & (int64_t)*(bytes+3)) << 24);
}

int64_t PacketSerizlizer::bytesToInt64(char* bytes)
{
    return ((0xff & (int64_t)*(bytes+0)) << 0)
           | ((0xff & (int64_t)*(bytes+1)) << 8)
           | ((0xff & (int64_t)*(bytes+2)) << 16)
           | ((0xff & (int64_t)*(bytes+3)) << 24)
           | ((0xff & (int64_t)*(bytes+4)) << 32)
           | ((0xff & (int64_t)*(bytes+5)) << 40)
           | ((0xff & (int64_t)*(bytes+6)) << 48)
           | ((0xff & (int64_t)*(bytes+7)) << 56);
}