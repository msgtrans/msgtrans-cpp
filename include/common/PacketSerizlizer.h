//
// Created by linsen on 2019/12/10.
//

#ifndef MSGTRANS_CLIENT_PACKETSERIZLIZER_H
#define MSGTRANS_CLIENT_PACKETSERIZLIZER_H

#include <vector>
#include <evpp/buffer.h>
#include "MessageBuffer.h"
//#include <memory>

#define MAX_NETPACK_SIZE	5242880
#define HEADER_LENGTH		16

struct Packet
{
    uint32_t messageId;
    uint32_t messsageLength;
   // int32_t message_data_length;
   // char    *message_data;
};



class PacketSerizlizer
{
public:
    PacketSerizlizer();

    ~PacketSerizlizer()
    {
        delete[] m_last_save_data;
    }
    /**
      * @brief 序列化message为有效的发送格式buffer
      * @param message   报文数据
      * @param length    序列化后buffer
      *
      * @return
      *  序列化后buffer size
      */
    static uint32_t  Serialize(std::shared_ptr<MessageBuffer> message, char*& serizelizeBuffer);

    /**
      * @brief 反序列化message
      * @param data         报文数据
      * @param data_size    数据长度
      * @param messages     反序列化后Message
      *
      * @return 成功 or 失败
      */
//    bool DeSerialize(const char* data, int data_size, std::vector<std::shared_ptr<MessageBuffer>> &messages,evpp::Buffer *msg);
    bool DeSerialize(const char* data, int data_size, std::vector<std::shared_ptr<MessageBuffer>> &messages);

private:
    int32_t bytesToInt32(char* bytes);
    int64_t bytesToInt64(char* bytes);

private:
    char *m_last_save_data;
    int  m_remain_size; // 当前数据的大小
};

#endif //MSGTRANS_CLIENT_PACKETSERIZLIZER_H
