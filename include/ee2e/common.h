/*
 * @Author: Dash Zhou
 * @Date: 2019-03-27 18:28:04
 * @Last Modified by:   Dash Zhou
 * @Last Modified time: 2019-03-27 18:28:04
 */

#ifndef __COMMON_H
#define __COMMON_H

#include <iostream>
#include <string>
#include "crypto.h"
#include "key_exchange.pb.h"
#include "common/base64.h"

namespace common
{

inline bool decrypt_ciphertext(const crypto::peerkey_s &peerkey, const ee2e::Ciphertext &ciphertext, std::string &plaintext)
{
//    std::cout << "AES IV:" << std::endl;
//    dash::hex_dump(ciphertext.aes_iv_12bytes());
//    std::cout << "AES TAG:" << std::endl;
//    dash::hex_dump(ciphertext.aes_tag_16bytes());
//    std::cout << "AES ciphertext:" << std::endl;
//    dash::hex_dump(ciphertext.ciphertext_nbytes());

    //std::string str_plaintext(ciphertext.ciphertext_nbytes().size(), '\0');
    // static bool Decode(const char *input, size_t input_length, char *out, size_t out_length)


    uint8_t * text = new uint8_t[ciphertext.cipher_version()];
    Base64::Decode(ciphertext.ciphertext_nbytes().c_str(),ciphertext.ciphertext_nbytes().length(), (char*)text,ciphertext.cipher_version());
    //Base64::Decode(ciphertext.ciphertext_nbytes() ,&text);
    uint8_t tag[CRYPTO_AES_TAG_LEN];
    Base64::Decode(ciphertext.aes_tag_16bytes().c_str(),ciphertext.aes_tag_16bytes().length(),(char*)tag,CRYPTO_AES_TAG_LEN);
    uint8_t iv[CRYPTO_AES_IV_LEN];
    Base64::Decode(ciphertext.aes_iv_12bytes().c_str(),ciphertext.aes_iv_12bytes().length(),(char*)iv,CRYPTO_AES_IV_LEN);

    std::cout << "ciphertext_nbytes:" << ciphertext.ciphertext_nbytes() << std::endl;
//    std::cout << "aes_tag_16bytes:" << ciphertext.aes_tag_16bytes() << std::endl;
//    std::cout << "aes_iv_12bytes:" << ciphertext.aes_iv_12bytes() << std::endl;

    printf("text : ");
    for(int i = 0 ; i < ciphertext.cipher_version() ; ++i)
    {
        printf("%d " ,text[i]);
    }
    printf("\n");
//
//
//    printf("tag : ");
//    for(int i = 0 ; i < CRYPTO_AES_TAG_LEN ; ++i)
//    {
//        printf(" %d  " ,tag[i]);
//    }
//    printf("\n");
//
//    printf("iv : ");
//    for(int i = 0 ; i < CRYPTO_AES_IV_LEN ; ++i)
//    {
//        printf(" %d  " ,iv[i]);
//    }
//    printf("\n");
//
//    printf("key : ");
//    for(int i = 0 ; i < CRYPTO_AES_KEY_LEN ; ++i)
//    {
//        printf(" %d  " ,peerkey.aes_key[i]);
//    }
//    printf("\n");

    bool ret = crypto::aes_decrypt((unsigned char*)text,
                                   ciphertext.cipher_version(),
                        (unsigned char*)tag,
                        peerkey.aes_key,
                        (unsigned char*)iv,
                        (unsigned char*)&plaintext[0]);
    delete [] text;
    if (!ret) return false;

   // std::cout << "Plaintext:" << plaintext << std::endl;
//    dash::hex_dump(str_plaintext);
//    if (!plaintext.ParseFromString(str_plaintext))
//    {
//        std::cout << "plaintext parsing error." << std::endl;
//        return false;
//    }

    return true;
}

inline bool encrypt_plaintext(const crypto::peerkey_s &peerkey, const std::string &str_plaintext, ee2e::Ciphertext &ciphertext)
{
    std::string str_ciphertext(str_plaintext.length(), '\0');
    uint8_t rand_iv[CRYPTO_AES_IV_LEN];
    uint8_t aes_tag[CRYPTO_AES_TAG_LEN];


    if (!crypto::rand_salt(rand_iv, CRYPTO_AES_IV_LEN))
    {
        return false;
    }

    bool ret = crypto::aes_encrypt((unsigned char *)str_plaintext.data(), str_plaintext.length(),
                                   peerkey.aes_key, rand_iv, (unsigned char *)&str_ciphertext[0], aes_tag);
    if (!ret)
    {
        return false;
    }
    std::cout << "=======================================================" << std::endl;

    printf("aes_key : ");
    for(int i = 0 ; i < CRYPTO_AES_KEY_LEN ; ++i)
    {
        printf("%d, " ,(uint8_t)peerkey.aes_key[i]);
    }
    printf("\n");

    printf("after str_ciphertext : ");
    for(int i = 0 ; i < str_plaintext.length() ; ++i)
    {
        printf("%d, " ,(uint8_t)str_ciphertext[i]);
    }
    printf("\n");

    printf("after iv : ");
    for(int i = 0 ; i < CRYPTO_AES_IV_LEN ; ++i)
    {
        printf("%d, " ,(uint8_t)rand_iv[i]);
    }
    printf("\n");

    printf("after tag : ");
    for(int i = 0 ; i < CRYPTO_AES_TAG_LEN ; ++i)
    {
        printf("%d, " ,(uint8_t)aes_tag[i]);
    }
    printf("\n");

    //ciphertext.set_cipher_version(CRYPTO_VERSION);

//    std::string pub(m_messageTransport->client_key.ec_pub_key , m_messageTransport->client_key.ec_pub_key + sizeof(m_messageTransport->client_key.ec_pub_key));
//    std::string out_pub;
//    std::string salt(m_messageTransport->client_key.salt,m_messageTransport->client_key.salt + sizeof(m_messageTransport->client_key.salt));
//    std::string out_salt;
//    Base64::Encode(pub,&out_pub);
//    Base64::Encode(salt,&out_salt);
    std::string iv;
    Base64::Encode(std::string(rand_iv,rand_iv + CRYPTO_AES_IV_LEN),&iv);
    std::cout << "base64 iv :" << iv << std::endl;
    std::string tag;
    Base64::Encode(std::string(aes_tag,aes_tag+ CRYPTO_AES_TAG_LEN),&tag);
    std::cout << "base64 tag :" << tag << std::endl;
    std::cout << "version :" << str_plaintext.length() << std::endl;

    ciphertext.set_aes_iv_12bytes(std::move(iv));
    ciphertext.set_aes_tag_16bytes(std::move(tag));
    std::string s_ciphertext;
    Base64::Encode(str_ciphertext,&s_ciphertext);

    std::cout << "base64 s_ciphertext :" << s_ciphertext << std::endl;


    ciphertext.set_ciphertext_nbytes(std::move(s_ciphertext));
    ciphertext.set_cipher_version(str_plaintext.length());

    return true;
}

inline bool verify_token(const uint8_t ecdh_pub_key[CRYPTO_EC_PUB_KEY_LEN], const ee2e::Token &token)
{
    uint8_t hmac_256[CRYPTO_HMAC_SHA256];

    bool ret = crypto::hmac_sha256(hmac_256,
         (uint8_t *)token.salt_3bytes().data(), token.salt_3bytes().size(),
         ecdh_pub_key, CRYPTO_EC_PUB_KEY_LEN);
    if (!ret)
    {
        std::cout << "hmac calculation error." << std::endl;
        return false;
    }

    if (0 != memcmp(token.hmac_3bytes().data(), hmac_256, 3))
    {
        std::cout << "Token check failed" << std::endl;
        return false;
    }

    return true;
}

inline bool generate_token(const uint8_t ecdh_pub_key[CRYPTO_EC_PUB_KEY_LEN], ee2e::Token &token)
{
    uint8_t random_digit[3];
    uint8_t hmac_256[CRYPTO_HMAC_SHA256];


    if (!crypto::rand_salt(random_digit, 3))
    {
        std::cout << "random digit generation error." << std::endl;
        return false;
    }

    if (!crypto::hmac_sha256(hmac_256, random_digit, 3, ecdh_pub_key, CRYPTO_EC_PUB_KEY_LEN))
    {
        std::cout << "hmac calculation error." << std::endl;
        return false;
    }

    std::string salt;
    Base64::Encode(std::string(random_digit,random_digit + 3),&salt);
    std::string hmac;
    Base64::Encode(std::string(hmac_256,hmac_256+3),&hmac);
    token.set_salt_3bytes(salt);
    token.set_hmac_3bytes(hmac);

    return true;
}

inline bool encrypted_encode(std::shared_ptr<MessageBuffer>& message,const ownkey_s& ownkey, const peerkey_s& peerkey)
{
    uint32_t  msgId = message->m_id;
    ee2e::Ciphertext *ciphertext = new ee2e::Ciphertext();
    if (!common::encrypt_plaintext(peerkey, message->m_data, *ciphertext))
    {
        std::cout << "aes encryption error." << std::endl;
        delete ciphertext;
        return false;
    }

    /* Generate the client's token */
    ee2e::Token *token = new ee2e::Token();
    if (!common::generate_token(ownkey.ec_pub_key, *token))
    {
        std::cout << "token generation error." << std::endl;
        delete token;
        return false;
    }
    ee2e::EncryptedRequest encrypted_request;
    encrypted_request.set_allocated_ciphertext(ciphertext);
    encrypted_request.set_allocated_token(token);
    std::string str_request;
    encrypted_request.SerializeToString(&str_request);
    message.reset(new MessageBuffer(msgId,str_request));
    return true;
}

inline bool encrypted_decode(std::shared_ptr<MessageBuffer>& message,const crypto::peerkey_s& peer_key)
{
    uint32_t id = message->m_id;
    ee2e::EncryptedRequest res;

//    printf("EncryptedRequest : ");
//    for(int i = 0 ; i < message->m_data.length() ; ++i)
//    {
//        printf(" %d  " ,message->m_data[i]);
//    }
//    printf("\n");


    if (!res.ParseFromString(message->m_data))
    {
        std::cerr << "Encryption response parsing error." << std::endl;
        return false;
    }
//    std::string ciphertext;
//    Base64::Decode(res.ciphertext().ciphertext_nbytes(),&ciphertext);

    std::string str_plaintext(res.ciphertext().cipher_version(), '\0');
    if (!common::decrypt_ciphertext(peer_key, res.ciphertext(), str_plaintext))
    {
        std::cout << "aes decryption error." << std::endl;
        return false;
    }
    message.reset(new MessageBuffer(id,str_plaintext));
    return true;
}


inline bool key_calculate(const crypto::ownkey_s &ownkey, crypto::peerkey_s &peerkey)
{
    /* XOR the ownkey and peerkey to one array */
    uint8_t salt_xor[CRYPTO_SALT_LEN];
    if (!crypto::bytes_xor(ownkey.salt, sizeof(crypto::ownkey_s::salt), peerkey.salt, sizeof(crypto::peerkey_s::salt), salt_xor))
    {
        std::cout << "xor calculation error." << std::endl;
        return false;
    }

    /* Calculate the shared key using own public and private keys and the public key of the other party */
    uint8_t shared_key[CRYPTO_ECDH_SHARED_KEY_LEN];
    if (!crypto::calc_ecdh_shared_key(ownkey.ec_pub_key, ownkey.ec_priv_key, peerkey.ec_pub_key, shared_key))
    {
        std::cout << "shared key calculation error." << std::endl;
        return false;
    }

    /* Using HKDF to calculate the final AES key */
    if (!crypto::generate_hkdf_bytes(shared_key, salt_xor, (uint8_t*)CRYPTO_KEY_INFO, strlen(CRYPTO_KEY_INFO), peerkey.aes_key))
    {
        std::cout << "hkdf calculation error." << std::endl;
        return false;
    }



    std::cout << "Calculated the final AES-KEY:" << std::endl;
   // dash::hex_dump(peerkey.aes_key, CRYPTO_AES_KEY_LEN, std::cout);

    return true;
}

}

#endif